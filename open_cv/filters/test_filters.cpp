#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

void write_img_with_caption(String &filename, Mat &src)
{
  putText( src, filename,
           Point( src.cols/4, src.rows/2),
           FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

  vector<int> compression_params;
  compression_params.push_back(IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);

  imwrite( filename, src , compression_params);
}


int main( void )
{
  Mat src; Mat dst;

  src = imread( "../lena.jpg", IMREAD_COLOR );
  if (src.data == NULL){
      std::cout << "n'do cazzo sta sta figura demmerda" << std::endl;
  }

  String filename("original_image.png");

  write_img_with_caption( filename, src);

  dst = src.clone();

  blur( src, dst, Size( 10, 10 ));
  filename = "blurred_10x10.png";
  write_img_with_caption( filename, dst);


  blur( src, dst, Size( 20, 20 ));
  filename = "blurred_20x20.png";
  write_img_with_caption( filename, dst);


  GaussianBlur( src, dst, Size( 10, 10 ), 0, 0 );
  filename = "gauss_blurred_20x20.png";
  write_img_with_caption( filename, dst);


  /*for (int i = 0; i < 31 ; i++){

      blur( src, dst, Size( i, i ), Point(-1,-1) );

  }*/



  /*if(  != 0 ) { return 0; }

  if( display_dst( DELAY_CAPTION ) != 0 ) { return 0; }
  if( display_caption( "Homogeneous Blur" ) != 0 ) { return 0; }
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
      {
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
  if( display_caption( "Gaussian Blur" ) != 0 ) { return 0; }
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
      { GaussianBlur( src, dst, Size( i, i ), 0, 0 );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
  if( display_caption( "Median Blur" ) != 0 ) { return 0; }
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
      { medianBlur ( src, dst, i );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
  if( display_caption( "Bilateral Blur" ) != 0 ) { return 0; }
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
      { bilateralFilter ( src, dst, i, i*2, i/2 );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
  display_caption( "End: Press a key!" );
  waitKey(0);*/
  return 0;
}
