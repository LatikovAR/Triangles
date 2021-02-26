cmake_minimum_required(VERSION 3.5)

project(Lib_geom LANGUAGES CXX)

add_library(geometry STATIC geometry_base.cpp geometry.cpp intersection_finder.cpp rotator.cpp)