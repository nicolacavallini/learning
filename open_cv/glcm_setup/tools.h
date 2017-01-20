#ifndef TOOLS_H
#define TOOLS_H

#include <cv.h>

#include <iostream>

#include <math.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace cv;

template<class T>
Mat_<T> gery_co_matrix(const Mat_<T> &image,
                    const double &distance,
                    const double &angle,
                    const T &levels)
{
    Mat_<T> output = Mat_<T>::zeros(image.rows,
                            image.cols);

    int delta_r = (int)round(sin(angle)*distance);
    int delta_c = (int)round(cos(angle)*distance);

    for (int r = 0; r< image.rows; r++){
        for (int c =0; c < image.cols; c++){

            T i = image(r,c);

            int d_row = r + delta_r;
            int d_col = c + delta_c;

            if (d_row >= 0 && d_row < image.rows &&
                d_col >= 0 && d_col < image.cols ){

                T j = image(d_row,d_col);

                if (i >= 0 && i < levels &&
                    j >= 0 && j < levels){

                    output(i,j)+=1;
                }
            }
        }
    }

    return output;
}

template<class T>
void explore_matrix(const Mat_<T> &image)
{
    int count= 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
    {
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        cout << "count = " << count;
        cout << ", i = " << i;
        cout << ", j = " << j;
        cout << ", element = " << *it << endl;
        count++;
    }

}

template<class T>
double evaluate_energy(const Mat_<T> &image)
{
    double energy = 0;

    auto it = image.begin();
    for (; it != image.end() ; ++it)
        energy+= (double)*it * (double)*it;

    return sqrt(energy);
}

template<class T>
double evaluate_entropy(const Mat_<T> &image)
{
    double entropy = 0;

    auto it = image.begin();
    for (; it != image.end() ; ++it)
        entropy+= (double)*it * log2((double)*it);

    return -1.*entropy;
}

template<class T>
Mat_<double> normalise_co_matrix(const Mat_<T> &image)
{
    double acc = 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
        acc+= (double)*it;

    Mat_<double> n(image.rows,image.cols);

    int count = 0;

    it = image.begin();
    for (; it != image.end() ; ++it){
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        n(i,j) = (double)*it/acc;
        count++;
    }
    return n;
}

template<class T>
Mat_<T> simmetrise_co_matrix( Mat_<T> &image)
{
    Mat_<T> n = image.clone();

    for (int i = 0; i<image.rows; i++)
        for (int j = 0; j<image.cols; j++)
            n(i,j)+=image(j,i);

    return n;
}

template<class input_format, class output_format>
void print_matrix(Mat_<input_format> &P)
{
    cout << "[";
    for (int i =0; i < P.rows; i++)
    {
        if (i!=0) cout << " ";
        for (int j =0; j < P.cols; j++)
        {
            cout << (output_format)P(i,j);
            if (j == P.cols-1 && i !=P.rows-1)
                cout << endl;
            else if (j == P.cols-1 && i ==P.rows-1)
                break;
            else
                cout << ", ";
        }
    }
    cout << "]" << endl;
}

#endif // TOOLS_H
