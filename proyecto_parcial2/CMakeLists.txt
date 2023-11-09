cmake_minimum_required(VERSION 3.26)
project(proyecto_parcial2)

set(CMAKE_CXX_STANDARD 17)

add_executable(proyecto_parcial2 main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(proyecto_parcial2 PRIVATE box2d::box2d)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(proyecto_parcial2 PRIVATE sfml-system sfml-network sfml-graphics sfml-window)