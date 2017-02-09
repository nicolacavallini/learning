CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT( DisplayImage )

SET(OpenCV_DIR "/home/nicola/local/usr/opencv-3.1.0/share/OpenCV"
   CACHE PATH "The path where OpenCVConfig.cmake is placed")
FIND_PACKAGE(OpenCV REQUIRED)


INCLUDE_DIRECTORIES( /home/nicola/local/usr/opencv-3.1.0/include )
ADD_EXECUTABLE( main main.cpp )
TARGET_LINK_LIBRARIES( main ${OpenCV_LIBS} )
