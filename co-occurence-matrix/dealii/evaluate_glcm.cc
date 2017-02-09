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
vector<GREY> get_grey_levels(FullMatrix<GREY> &image,
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

    vector<size_m> start_pixel;
    vector<size_m> end_pixel;

    evaluate_spatial_roule(m.m(),m.n(),distance,angle,
                           start_pixel,end_pixel);

    cout << "# of spatial rules = " << start_pixel.size() << endl;

    auto start_grey_levels = get_grey_levels(m,start_pixel);
    auto end_grey_levels = get_grey_levels(m,end_pixel);

    SparsityPattern start_sp;
    size_m rows = (size_m)start_pixel.size();
    size_m cols = (size_m)g_levels;
    start_sp.reinit(rows,cols,1);

    size_m rid = 0;
    for(auto const& cid: start_grey_levels)
    {
        cout << "start_grey_levels = " << cid << endl;
        start_sp.add(rid,cid);
        rid+=1;
    }





    std::ofstream out ("sparsity_pattern.svg");
    start_sp.print_svg (out);


    cout << "stocazzo" << endl;
    return 0;
}
