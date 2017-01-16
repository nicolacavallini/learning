cmake_minimum_required(VERSION 2.8.9)
message(${CMAKE_SOURCE_DIR})
message(${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/stocazzo
     DESTINATION ${CMAKE_BINARY_DIR}/)
project (hello)
add_executable(hello main.cpp)
