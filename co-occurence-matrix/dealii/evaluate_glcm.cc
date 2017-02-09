#include <fstream>
#include <iostream>
#include <math.h>
#include <array>

#include <deal.II/base/logstream.h>
#include <deal.II/lac/sparsity_pattern.h>

#include <deal.II/numerics/data_out.h>

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/full_matrix.templates.h>

#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/sparse_matrix.templates.h>

using namespace dealii;

template class FullMatrix<int>;
template class SparseMatrix<int>;

using namespace std;

typedef unsigned int size_m;
typedef int gl_t;

template <typename number>
void
print_matrix (const FullMatrix<number > &m)
{
    for (unsigned int i=0; i<m.m(); ++i) {
        for (unsigned int j=0; j<m.n(); ++j)
            cout <<  m(i,j) << ", ";
        cout << std::endl;
    }
}

template<class SIZE_T>
void evaluate_spatial_roule(const SIZE_T rows, const SIZE_T cols,
                            const double distance,
                            const double angle,
                            vector<SIZE_T> &start_pixel,
                            vector<SIZE_T> &end_pixel)
{
    SIZE_T delta_r = (SIZE_T)round(sin(angle)*distance);
    SIZE_T delta_c = (SIZE_T)round(cos(angle)*distance);

    for (SIZE_T r = 0; r< rows; r++) {
        for (SIZE_T c =0; c < cols; c++) {

            SIZE_T d_row = r + delta_r;
            SIZE_T d_col = c + delta_c;

            if (d_row >= 0 && d_row < rows &&
                    d_col >= 0 && d_col < cols ) {

                SIZE_T start_pixel_id = r * cols + c;
                SIZE_T end_pixel_id = d_row * cols + d_col;

                start_pixel.push_back(start_pixel_id);
                end_pixel.push_back(end_pixel_id);
            }
        }
    }
}

template<class SIZE_T>
array<SIZE_T,2> raw_id_to_ij(SIZE_T raw_id, SIZE_T cols)
{
    array<SIZE_T,2> ij;
    ij[0] = floor(raw_id/cols);
    ij[1] = raw_id - ij[0]*cols;

    return ij;
}

template<class GREY, class SIZE_T>
vector<GREY> get_grey_levels(const FullMatrix<GREY> &image,
                             vector<SIZE_T> &raw_id)
{
    vector<GREY> grey_levels;

    for(auto const& rid: raw_id)
    {
        auto ij = raw_id_to_ij(rid,image.n());
        grey_levels.push_back(image(ij[0],ij[1]));
    }
    return grey_levels;
}

template<class GREY, class SIZE_T>
void make_connectivity_matrix(const vector<SIZE_T> &pixel_position,
                              const vector<GREY> &pixel_grey_levels,
                              const GREY g_levels,
                              SparsityPattern &sp,
                              SparseMatrix<GREY> &connectivity_matrix)
{

    SIZE_T rows = (SIZE_T)pixel_position.size();
    SIZE_T cols = (SIZE_T)g_levels;
    sp.reinit(rows,cols,1);

    SIZE_T rid = 0;
    for(auto const& cid: pixel_grey_levels)
    {
        sp.add(rid,cid);
        rid+=1;
    }

    sp.compress();
    connectivity_matrix.reinit(sp);

    rid = 0;
    for(auto const& cid: pixel_grey_levels)
    {
        connectivity_matrix.set(rid,cid,1);
        rid+=1;
    }

}

template<class GREY, class SIZE_T>
void make_glcm(const FullMatrix<GREY> &m,
               const GREY g_levels,
               const double distance,
               const double angle,
               SparsityPattern &glcm_sp,
               SparseMatrix<GREY> &glcm)
{
    vector<SIZE_T> start_pixel;
    vector<SIZE_T> end_pixel;

    evaluate_spatial_roule<SIZE_T>(m.m(),m.n(),distance,angle,
                           start_pixel,end_pixel);

    //cout << "# of spatial rules = " << start_pixel.size() << endl;

    auto start_grey_levels = get_grey_levels<GREY, SIZE_T>(m,start_pixel);
    auto end_grey_levels = get_grey_levels<GREY, SIZE_T>(m,end_pixel);

    SparsityPattern start_sp;
    SparseMatrix<GREY> start_connectivity_matrix;
    make_connectivity_matrix<GREY, SIZE_T>(start_pixel,start_grey_levels,g_levels,
                             start_sp,start_connectivity_matrix);

    SparsityPattern end_sp;
    SparseMatrix<GREY> end_connectivity_matrix;
    make_connectivity_matrix<GREY, SIZE_T>(end_pixel,end_grey_levels,g_levels,
                             end_sp,end_connectivity_matrix);

    glcm_sp.reinit(g_levels,g_levels,1);
    glcm_sp.compress();
    glcm.reinit(glcm_sp);

    start_connectivity_matrix.print(cout);
    end_connectivity_matrix.print(cout);

    start_connectivity_matrix.Tmmult(glcm,end_connectivity_matrix);
}


int main()
{
    FullMatrix<gl_t> m;
    m.reinit(4,4);
    m(0,0) = 0; m(0,1) = 0; m(0,2) = 1; m(0,3) = 1;
    m(1,0) = 0; m(1,1) = 0; m(1,2) = 1; m(1,3) = 1;
    m(2,0) = 0; m(2,1) = 2; m(2,2) = 2; m(2,3) = 2;
    m(3,0) = 2; m(3,1) = 2; m(3,2) = 3; m(3,3) = 3;

    gl_t g_levels = 4;

    print_matrix(m);

    const double pi = M_PI;

    double angle = pi/2;
    double distance = 1;

    SparsityPattern glcm_sp;
    SparseMatrix<gl_t> glcm;

    make_glcm<gl_t,size_m>(m,g_levels,distance,angle,glcm_sp,glcm);

    glcm.print(cout);

    return 0;
}
