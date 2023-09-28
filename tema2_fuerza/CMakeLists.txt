cmake_minimum_required(VERSION 3.26)
project(tema2_fuerza)

set(CMAKE_CXX_STANDARD 17)

add_executable(tema2_fuerza main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(tema2_fuerza PRIVATE box2d::box2d)