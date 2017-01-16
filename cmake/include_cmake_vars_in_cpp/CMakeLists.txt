cmake_minimum_required (VERSION 2.6)
project (cmake_vars_in_code)

set( myvar "somebody" )

# Replaces occurrences of @cmake_variable@ with the variable's contents.
# Input is ${CMAKE_SOURCE_DIR}/main.cpp.in,
# output is ${CMAKE_BINARY_DIR}/main.cpp
configure_file( vars.h.in vars.h @ONLY )

include_directories(${CMAKE_BINARY_DIR})

add_executable(cmake_vars_in_code main.cpp)
