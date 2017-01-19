#ifndef TOOLS_H
#define TOOLS_H

#include <cv.h>

#include <iostream>

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

    double dr = round(sin(angle)*distance);
    double dc = round(cos(angle)*distance);

    cout << "dr = " << dr << endl;
    cout << "dc = " << dc << endl;

    int delta_r = (int)dr;
    int delta_c = (int)dc;

    cout << "delta_r = " << delta_r << endl;
    cout << "delta_c = " << delta_c << endl;


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
