cmake_minimum_required(VERSION 3.8)

project(GammaAlgorithm)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(.)
add_subdirectory(gamma)
