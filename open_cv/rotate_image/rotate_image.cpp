#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int display_and_wait(const std::string &windowName,
                     const cv::Mat & image,
                     double scale = 1.0,
                     int colormap = -1,
                     int window_type = CV_WINDOW_AUTOSIZE) {
      cv::Mat tmp, tmp_;
      double min_;
      double max_;

     cv::resize(image, tmp, cv::Size(0, 0), scale, scale);

     tmp.copyTo(tmp_);

     if (tmp.depth() != CV_8U) {
        cv::minMaxIdx(tmp, &min_, &max_);
        tmp -= min_;
        tmp.convertTo(tmp_, CV_8UC1, 255 / (max_ - min_));
      }

     if (colormap != -1)
        cv::applyColorMap(tmp_, tmp_, colormap);

     cv::namedWindow(windowName, window_type);
      cv::imshow(windowName, tmp_);

     cv::waitKey();
     cv::destroyWindow(windowName);
}

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

int main()
{
    Mat src = imread("/home/nicola/local/learning/open_cv/rotate_image/CheHigh.jpg");

    Mat dst;
    dst = rotate(src, 10);

    int err = display_and_wait("src",src,.2);
    err = display_and_wait("dst",dst,.2);

    return 0;
}
