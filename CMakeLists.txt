cmake_minimum_required(VERSION 3.5)

project(2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O3")

find_package(Vulkan REQUIRED)

add_subdirectory(Geometry)

add_library(vulkan_visualization STATIC vulkan_drawing.cpp)
target_link_libraries(vulkan_visualization geometry)

target_include_directories(vulkan_visualization PRIVATE
    $ENV{VULKAN_SDK}/Include
    ${GLFW}/include
    ${GLM})

target_link_libraries(vulkan_visualization
    $ENV{VULKAN_SDK}/Lib/vulkan-1.lib
    ${GLFW}/lib-mingw-w64/libglfw3.a)

add_custom_command(TARGET vulkan_visualization
                   COMMAND $ENV{VULKAN_SDK}/Bin/glslc.exe
                   ARGS shaders/shader.frag -o frag.spv
                   COMMAND $ENV{VULKAN_SDK}/Bin/glslc.exe
                   ARGS shaders/shader.vert -o vert.spv)

add_executable(2 main.cpp)
target_link_libraries(2 vulkan_visualization)
