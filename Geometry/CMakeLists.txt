cmake_minimum_required(VERSION 3.5)

project(Lib_geom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(geometry STATIC geometry_base.cpp geometry.cpp intersection_finder.cpp rotator.cpp)
