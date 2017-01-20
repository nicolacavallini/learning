#include <iostream>

#include <cv.h>

#include <math.h>

#include "./tools.h"

using namespace std;
using namespace cv;


template<class T>
void run_test(double &angle, double &distance)
{
    Mat_<T> test_image = (Mat_<T>(4,4)
              << 0, 0, 1, 1,0, 0, 1, 1,0, 2, 2, 2,2, 2, 3, 3);

    cout << test_image;

    T levels = 4;

    cout << "test_image.rows = " << test_image.rows << endl;
    cout << "test_image.cols = " << test_image.cols << endl;

    Mat_<T> glcm0 = gery_co_matrix(test_image,distance,angle,levels);
    print_matrix<T,int>(glcm0);

    Mat_<double> glcm_n = normalise_co_matrix(glcm0);
    print_matrix<double,double>(glcm_n);

    Mat_<T> glcm_s = simmetrise_co_matrix(glcm0);
    print_matrix<T,int>(glcm_s);

    Mat_<double> glcm_ns = simmetrise_co_matrix(glcm_n);
    print_matrix<double,double>(glcm_ns);

    cout << "energy normed symmetric = " << evaluate_energy(glcm_ns) << endl;
    cout << "energy normed = " << evaluate_energy(glcm_n) << endl;
    cout << "energy not normed= " << evaluate_energy(glcm0) << endl;


}

int main(){

    const double pi = M_PI;

    double angle = pi/6.+.0000000001;
    double distance = 1;
    //run_test<uint8_t>(angle, distance);
    run_test<uint16_t>(angle, distance);

}
