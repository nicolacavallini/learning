#ifndef MATRIX_TOOLS_H
#define MATRIX_TOOLS_H

#include <cv.h>

#include <iostream>

using namespace cv;
using namespace std;

template<class input_format, class output_format>
void print_matrix(const Mat_<input_format> &P);

template<class T>
Mat_<T> create_i_long_column(int l);

template<class T>
Mat_<T> create_j_long_row(int l);

template<class T>
T scalar_prod(Mat_<T> &row, Mat_<T> &col);

template<class T>
Mat_<T> mat_mat_mult(Mat_<T> &row, Mat_<T> &col);

template<class T>
Mat_<T> mult_row_by_i(const Mat_<T> &row);

template<class T>
Mat_<T> mult_col_by_j(const Mat_<T> &row);

template<class T>
Mat_<T> elem_elem_prod(Mat_<T> &row, Mat_<T> &col);

template<class T>
Mat_<T> shif_by_scalar(Mat_<T> &row, T shift, bool negative=true);

vector<vector<bool>> mask_zero_std(vector<double> &std_i ,
                                   vector<double> &std_j);

template<class T>
T sum_ij(const Mat_<T> &image);

#endif // MATRIX_TOOLS_H
