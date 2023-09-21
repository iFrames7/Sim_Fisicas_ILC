cmake_minimum_required(VERSION 3.26)
project(hola_luna)

set(CMAKE_CXX_STANDARD 17)

add_executable(hola_luna main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(hola_luna PRIVATE box2d::box2d)
