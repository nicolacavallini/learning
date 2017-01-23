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
    for (; ia != a.end() ; ++ia, ++ib){
        EXPECT_EQ(*ia,*ib);
    }
}



/*TEST(TestGLCM, test_glcm)
{
    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;

    Mat_<uint16_t> test_image = (Mat_<uint16_t>(4,5) <<
                 0, 0, 1, 1, 1,
                 0, 0, 1, 1, 1,
                 0, 2, 2, 2, 3,
                 2, 2, 3, 3, 3);

    cout << test_image;

    uint16_t levels = 4;

    Mat_<uint16_t> glcm0 = gery_co_matrix(test_image,distance,angle,levels);
    cout << "glcm0 = " << endl;
    cout << glcm0 << endl;

    Mat_<uint16_t> expected_co_matrix = ( Mat_<uint16_t>(4,5) <<
                                          2, 1, 3, 0, 0,
                                          1, 4, 1, 1, 0,
                                          3, 1, 0, 3, 0,
                                          0, 1, 3, 0, 0);

    compare_matrices(glcm0,expected_co_matrix);
}*/

/*TYPED_TEST(TestFoo, test1)
{
    EXPECT_TRUE(test_equal<TypeParam>());
}*/

//template<class T>
//void test_square(double &angle, double &distance)
TEST(TestGLCM, test_glcm)
{
    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;

    Mat_<uint16_t> test_image = (Mat_<uint16_t>(4,4) <<
                 0, 0, 1, 1,
                 0, 0, 1, 1,
                 0, 2, 2, 2,
                 2, 2, 3, 3);

    uint16_t levels = 4;

    Mat_<uint16_t> glcm0 = gery_co_matrix(test_image,distance,angle,levels);

    Mat_<uint16_t> expected_co_matrix = ( Mat_<uint16_t>(4,4) <<
                                1, 1, 3, 0,
                                0, 1, 1, 0,
                                0, 0, 0, 2,
                                0, 0, 0, 0);

    compare_matrices_int(glcm0,expected_co_matrix);

    //compare_matrices(gery_co_matrix,expected_co_matrix);

    /*Mat_<double> glcm_n = normalise_co_matrix(glcm0);
    print_matrix<double,double>(glcm_n);

    Mat_<T> glcm_s = simmetrise_co_matrix(glcm0);
    print_matrix<T,int>(glcm_s);

    Mat_<double> glcm_ns = simmetrise_co_matrix(glcm_n);
    print_matrix<double,double>(glcm_ns);

    cout << "energy normed symmetric = " << evaluate_energy(glcm_ns) << endl;
    cout << "energy normed = " << evaluate_energy(glcm_n) << endl;
    cout << "energy not normed= " << evaluate_energy(glcm0) << endl;*/
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
