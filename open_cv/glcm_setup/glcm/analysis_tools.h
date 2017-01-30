#ifndef ANALYSIS_TOOLS_H
#define ANALYSIS_TOOLS_H

#include <cv.h>

#include <iostream>

using namespace cv;
using namespace std;

template<class T>
Mat_<T> gery_co_matrix(const Mat_<T> &image,
                    const double &distance,
                    const double &angle,
                    const int &levels);

template<class T>
void explore_matrix(const Mat_<T> &image);

template<class T>
double evaluate_energy(const Mat_<T> &image);

template<class T>
double evaluate_contrast(const Mat_<T> &image);

template<class T>
double evaluate_homogeneity(const Mat_<T> &image);

template<class T>
double evaluate_dissimilarity(const Mat_<T> &image);

template<class  T>
Mat_<T> evaluate_diff(Mat_<T> &image, string dir = "i");

template<class  T>
double evaluate_std(Mat_<T> &image, string dir = "i");

template<class  T>
T evaluate_covariance(Mat_<T> &image);

template<class  T>
double evaluate_correlation(Mat_<T> &image);

template<class T>
double evaluate_entropy(const Mat_<T> &image);

template<class T>
Mat_<double> normalise_co_matrix(const Mat_<T> &image);

template<class T>
Mat_<T> simmetrise_co_matrix( Mat_<T> &image);

#endif // MATRIX_TOOLS_H
