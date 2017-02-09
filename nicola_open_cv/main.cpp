#include <iostream>

#include <cv.h>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;

int main()
{
    Mat_<int> ex_col = (Mat_<int>(5,1) <<
                         0, 1, 2, 3, 4);
    cout << "stocazzo" << endl;

    cout << ex_col << endl;
    return 0;
}
