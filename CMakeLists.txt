cmake_minimum_required(VERSION 3.5)

project(2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VULKAN D:/Vulkan/1.2.162.1)
set(GLFW D:/glfw-3.3.2.bin.WIN64)
set(GLM D:/glm)

add_library(geometry STATIC geometry.cpp)
add_library(vulkan_visualization STATIC vulkan_drawing.cpp)

target_include_directories(vulkan_visualization PRIVATE
    ${VULKAN}/Include
    ${GLFW}/include
    ${GLM})

target_link_libraries(vulkan_visualization
    ${VULKAN}/Lib/vulkan-1.lib
    ${GLFW}/lib-mingw-w64/libglfw3.a)

add_custom_command(TARGET vulkan_visualization
                   COMMAND ${VULKAN}/Bin/glslc.exe
                   ARGS shaders/shader.frag -o frag.spv
                   COMMAND ${VULKAN}/Bin/glslc.exe
                   ARGS shaders/shader.vert -o vert.spv)

add_executable(2 main.cpp)

target_link_libraries(2 geometry vulkan_visualization)
