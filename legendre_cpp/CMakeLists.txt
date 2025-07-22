cmake_minimum_required(VERSION 3.5.0)
project(HELLO)

set(CMAKE_CXX_STANDARD 23)

include_directories( ${Boost_INCLUDE_DIR} )

add_executable (main main.cpp)

find_package(Boost REQUIRED COMPONENTS program_options HINTS "/opt/homebrew/Cellar/boost/1.85.0/lib/cmake/Boost-1.85.0")

target_link_libraries(main LINK_PUBLIC ${Boost_LIBRARIES} )

message(STATUS "---------------------")
message(STATUS "Boost_FOUND: ${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "---------------------")