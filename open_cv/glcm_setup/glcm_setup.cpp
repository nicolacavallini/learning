#include <iostream>
#include <math.h>

#include <cv.h>

using namespace std;
using namespace cv;

Mat_<uint8_t> gery_co_matrix(const Mat_<uint8_t> &image,
                    const double &distance,
                    const double &angle,
                    const uint8_t &levels)
{
    Mat_<uint8_t> output = Mat_<uint8_t>::zeros(image.rows,
                            image.cols);

    for (int r = 0; r< image.rows; r++){
        for (int c =0; c < image.cols; c++){

            uint8_t i = image.at<uint8_t>(r,c);

            int d_row = r + (int)round(sin(angle)*distance);
            int d_col = c + (int)round(cos(angle)*distance);

            if (d_row >= 0 && d_row < image.rows &&
                d_col >= 0 && d_col < image.cols ){

                uint8_t j = image.at<uint8_t>(d_row,d_col);

                if (i >= 0 && i < levels &&
                    j >= 0 && j < levels){

                    output.at<uint8_t>(i,j)+=1;
                }
            }
        }
    }

    return output;
}

int main(){

    const double pi = atan(1.0)*4;

    Mat_<uint8_t> test_image = (Mat_<uint8_t>(4,4) << 0, 0, 1, 1,0, 0, 1, 1,0, 2, 2, 2,2, 2, 3, 3);
    cout << "test_image = " << endl << " " << test_image << endl << endl;

    Mat_<uint8_t> output = Mat_<uint8_t>::zeros(test_image.rows,
                            test_image.cols);

    double angle = pi/4;
    double distance = 1;

    u_int8_t levels = 4;

    cout << "test_image.rows = " << test_image.rows << endl;
    cout << "test_image.cols = " << test_image.cols << endl;

    Mat_<uint8_t> glcm = gery_co_matrix(test_image,distance,angle,levels);

    cout << "output = " << endl << glcm << endl;
}
