#include <iostream>

#include "gtest/gtest.h"

#include <cv.h>

#include <math.h>

#include "./la/matrix_tools.h"
#include "./glcm/analysis_tools.h"

using namespace std;
using namespace cv;

typedef int img_data;

template<class T>
void compare_matrices_int(const T &a,
                      const T &b)
{
    EXPECT_EQ(a.rows,b.rows);
    EXPECT_EQ(a.cols,b.cols);
    auto ia = a.begin();
    auto ib = b.begin();
    for (; ia != a.end() ; ++ia, ++ib)
        EXPECT_EQ(*ia,*ib);
}

template<class T>
void compare_matrices_double(const T &a,
                      const T &b)
{
    EXPECT_EQ(a.rows,b.rows);
    EXPECT_EQ(a.cols,b.cols);
    auto ia = a.begin();
    auto ib = b.begin();
    for (; ia != a.end() ; ++ia, ++ib)
        EXPECT_NEAR(*ia,*ib,1E-10);
}

TEST(TestGLCM, basic_tools)
{
    Mat_<img_data> col = create_i_long_column<img_data>(5);

    Mat_<img_data> ex_col = (Mat_<img_data>(5,1) <<
                             0, 1, 2, 3, 4);
    compare_matrices_int(col,ex_col);

    Mat_<img_data> row = create_j_long_row<img_data>(5);

    Mat_<img_data> ex_row = (Mat_<img_data>(1,5) <<
                             0, 1, 2, 3, 4);
    compare_matrices_int(row,ex_row);

    img_data s = scalar_prod(row,col);

    EXPECT_EQ(s,30);

    Mat_<img_data> mm = mat_mat_mult(col,row);

    Mat_<img_data> mm_ex = (Mat_<img_data>(5,5) <<
                             0, 0, 0, 0, 0,
                             0, 1, 2, 3, 4,
                             0, 2, 4, 6, 8,
                             0, 3, 6, 9, 12,
                             0, 4, 8, 12, 16);

    compare_matrices_int(mm,mm_ex);

    Mat_<img_data> uno = Mat_<img_data>::ones(5,5);

    Mat_<img_data> el_el_prod = elem_elem_prod(uno,mm);

    compare_matrices_int(mm_ex,el_el_prod);

    Mat_<img_data> shifted = shif_by_scalar<img_data>(uno,1);

    Mat_<img_data> zero = Mat_<img_data>::zeros(5,5);

    compare_matrices_int(shifted,zero);

    Mat_<img_data> test = (Mat_<img_data>(4,4) <<
                 1, 2, 3, 4,
                 5, 6, 7, 8,
                 9,10,11,12,
                13,14,15,16);

    Mat_<img_data> test_i = mult_row_by_i(test);

    Mat_<img_data> ti_ex =  (Mat_<img_data>(4,4) <<
                             0, 0, 0, 0,
                             5, 6, 7, 8,
                             18, 20, 22, 24,
                             39, 42, 45, 48);

    compare_matrices_int(test_i,ti_ex);

    Mat_<img_data> test_j = mult_col_by_j(test);

    Mat_<img_data> tj_ex =  (Mat_<img_data>(4,4) <<
                             0, 2, 6, 12,
                             0, 6, 14, 24,
                             0, 10, 22, 36,
                             0, 14, 30, 48);

    compare_matrices_int(test_j,tj_ex);
}



TEST(TestGLCM, std_deviation)
{
    Mat_<img_data> test_image = (Mat_<img_data>(4,4) <<
                                  1, 1, 3, 0,
                                  0, 1, 1, 0,
                                  0, 0, 0, 2,
                                  0, 0, 0, 0);

    double std_i = evaluate_std(test_image,"i");

    double std_j = evaluate_std(test_image,"j");


    EXPECT_NEAR(16.18641406,std_i,1E-8);
    EXPECT_NEAR(42.75511665,std_j,1E-8);
}

void print_mask(vector<vector<bool>> &mask,
                bool print_alpha=true)
{
    auto ir = mask.begin();
    for (; ir != mask.end(); ++ir){
        auto ic = ir->begin();
        for (; ic != ir->end(); ++ic){
            if (print_alpha)
                cout << boolalpha;
            cout << *ic << ", ";
        }
        cout << endl;
    }
}

template<class T>
void compare_vec_of_vec(const vector<vector<T>> &mask1,
                        const vector<vector<T>> &mask2){
    //TODO assetion on dimensions
    auto ir1 = mask1.begin();
    auto ir2 = mask2.begin();
    for (; ir1!=mask1.end();++ir1, ++ir2){
        auto ic1 = ir1->begin();
        auto ic2 = ir2->begin();
        for (; ic1 != ir1->end(); ++ic1, ++ic2)
            EXPECT_EQ(*ic1,*ic2);
    }
}

TEST(TestGLCM, mask)
{
    vector<double> std_i = {2.2, 4.1, 5., 0, 6.3};
    vector<double> std_j = {2.3, 5.3, 0, 3.1, 9.3};


    vector<vector<bool>> mask = mask_zero_std(std_i,std_j);

    vector<vector<bool>> mask_ex = {
        {true, true, false, true, true},
        {true, true, false, true, true},
        {true, true, false, true, true},
        {false, false, false, false, false},
        {true, true, false, true, true}};

    compare_vec_of_vec(mask,mask_ex);
    //EXPECT_DOUBLE_EQ(10, evaluate_mean(col_j));

    //EXPECT_NEAR(1.11803398874989,evaluate_stdv(row_i),1E-10);

    //EXPECT_NEAR(4.47213595499958,evaluate_stdv(col_j),1E-10);
}


TEST(TestGLCM, test_glcm)
{
    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;

    img_data levels = 4;

    Mat_<img_data> test_image = (Mat_<img_data>(levels,levels) <<
                 0, 0, 1, 1,
                 0, 0, 1, 1,
                 0, 2, 2, 2,
                 2, 2, 3, 3);



    Mat_<img_data> glcm0 = gery_co_matrix(test_image,distance,angle,levels);

    Mat_<img_data> expected_co_matrix = ( Mat_<img_data>(levels,levels) <<
                                1, 1, 3, 0,
                                0, 1, 1, 0,
                                0, 0, 0, 2,
                                0, 0, 0, 0);

    compare_matrices_int(glcm0,expected_co_matrix);

    Mat_<double> glcm_n = normalise_co_matrix(glcm0);

    Mat_<double> expected_co_matrix_n = ( Mat_<double>(levels,levels) <<
     0.111111111111111, 0.111111111111111, 0.333333333333333, 0,
     0, 0.111111111111111, 0.111111111111111, 0,
     0, 0, 0, 0.222222222222222,
     0, 0, 0, 0);

    compare_matrices_double(glcm_n,expected_co_matrix_n);

    Mat_<img_data> glcm_s = simmetrise_co_matrix(glcm0);

    Mat_<img_data> expected_co_matrix_s = ( Mat_<img_data>(levels,levels) <<
                                          2, 1, 3, 0,
                                          1, 2, 1, 0,
                                          3, 1, 0, 2,
                                          0, 0, 2, 0);

    compare_matrices_int(glcm_s, expected_co_matrix_s);

    Mat_<double> glcm_ns = simmetrise_co_matrix(glcm_n);

    Mat_<double> expected_co_matrix_ns = ( Mat_<double>(levels,levels) <<
    0.222222222222222, 0.111111111111111, 0.333333333333333, 0,
     0.111111111111111, 0.222222222222222, 0.111111111111111, 0,
     0.333333333333333, 0.111111111111111, 0, 0.222222222222222,
     0, 0, 0.222222222222222, 0);

    compare_matrices_double(glcm_s, expected_co_matrix_s);

    EXPECT_DOUBLE_EQ(0.684934889218775,evaluate_energy(glcm_ns));
    EXPECT_DOUBLE_EQ(0.458122847290851,evaluate_energy(glcm_n));
    EXPECT_DOUBLE_EQ(4.12310562561766,evaluate_energy(glcm0));

    double contrast = evaluate_contrast(glcm0);
    EXPECT_DOUBLE_EQ(16,contrast);

    double dissimilarity = evaluate_dissimilarity(glcm0);
    EXPECT_DOUBLE_EQ(10,dissimilarity);

    double homogeneity = evaluate_homogeneity(glcm0);
    EXPECT_DOUBLE_EQ(4.6,homogeneity);

    double correlation = evaluate_correlation(glcm0);
    EXPECT_NEAR(correlation,0.99269994,1E-8);
}

typedef ::testing::Types<img_data> MyTypes;
TYPED_TEST_CASE(NumTest, MyTypes);

int main(int argc, char **argv){

    ::testing::InitGoogleTest(&argc, argv);

    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;

    //test_square<img_data>(angle, distance);

    return RUN_ALL_TESTS();
}
