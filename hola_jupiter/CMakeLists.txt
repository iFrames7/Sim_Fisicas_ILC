cmake_minimum_required(VERSION 3.26)
project(hola_jupiter)

set(CMAKE_CXX_STANDARD 17)

add_executable(hola_jupiter main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(hola_jupiter PRIVATE box2d::box2d)