cmake_minimum_required(VERSION 2.8)
project( glcm_setup )
set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
find_package( OpenCV REQUIRED )
find_package(Boost REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${Boost_INCLUDE_DIRS})

set(GTEST_INCLUDE_DIRS $ENV{GTEST_INCLUDE_DIR})
set(GTEST_LIBRARY $ENV{GTEST_LIBRARY})
set(GTEST_MAIN_LIBRARY $ENV{GTEST_MAIN_LIBRARY})
set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
set(GTEST_FOUND TRUE)

find_package (Threads)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable( glcm_setup glcm_setup.cpp )
target_link_libraries( glcm_setup ${OpenCV_LIBS} )
target_link_libraries( glcm_setup ${Boost_LIBRARIES})
target_link_libraries( glcm_setup ${GTEST_BOTH_LIBRARIES})
target_link_libraries( glcm_setup ${CMAKE_THREAD_LIBS_INIT})
