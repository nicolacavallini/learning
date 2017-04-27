cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(pcl_viewer)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (pcl_viewer pcl_visualizer.cpp)
target_link_libraries (pcl_viewer ${PCL_LIBRARIES})
