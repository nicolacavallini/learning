#include <iostream>

#include "gtest/gtest.h"

#include <cv.h>

#include <math.h>

#include "./tools.h"

using namespace std;
using namespace cv;

template<class T>
void compare_matrices_int(const T &a,
                      const T &b)
{
    auto ia = a.begin();
    auto ib = b.begin();
    for (; ia != a.end() ; ++ia, ++ib)
        EXPECT_EQ(*ia,*ib);
}

template<class T>
void compare_matrices_double(const T &a,
                      const T &b)
{
    auto ia = a.begin();
    auto ib = b.begin();
    for (; ia != a.end() ; ++ia, ++ib)
        EXPECT_NEAR(*ia,*ib,1E-10);
}

TEST(TestGLCM, mean_std_deviation)
{
    Mat_<uint16_t> test_image = (Mat_<uint16_t>(4,4) <<
                 1, 2, 3, 4,
                 5, 6, 7, 8,
                 9,10,11,12,
                13,14,15,16);

    Mat_<uint16_t> row_i= test_image.row(3);
    Mat_<uint16_t> col_j = test_image.col(3);

    //EXPECT_DOUBLE_EQ(14.5, evaluate_mean(row_i));
    //EXPECT_DOUBLE_EQ(10, evaluate_mean(col_j));

    //EXPECT_NEAR(1.11803398874989,evaluate_stdv(row_i),1E-10);

    //EXPECT_NEAR(4.47213595499958,evaluate_stdv(col_j),1E-10);
}


TEST(TestGLCM, test_glcm)
{
    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;

    uint16_t levels = 4;

    Mat_<uint16_t> test_image = (Mat_<uint16_t>(levels,levels) <<
                 0, 0, 1, 1,
                 0, 0, 1, 1,
                 0, 2, 2, 2,
                 2, 2, 3, 3);



    Mat_<uint16_t> glcm0 = gery_co_matrix(test_image,distance,angle,levels);

    Mat_<uint16_t> expected_co_matrix = ( Mat_<uint16_t>(levels,levels) <<
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

    Mat_<uint16_t> glcm_s = simmetrise_co_matrix(glcm0);

    Mat_<uint16_t> expected_co_matrix_s = ( Mat_<uint16_t>(levels,levels) <<
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

    cout << "correlation = " << correlation << endl;
}

typedef ::testing::Types<uint16_t> MyTypes;
TYPED_TEST_CASE(NumTest, MyTypes);

int main(int argc, char **argv){

    ::testing::InitGoogleTest(&argc, argv);

    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;

    //test_square<uint16_t>(angle, distance);

    return RUN_ALL_TESTS();
}
