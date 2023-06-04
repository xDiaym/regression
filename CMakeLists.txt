cmake_minimum_required(VERSION 3.26)
project(linear_regression)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(include)
add_executable(linear_regression main.cpp src/linalg.cpp src/regression.cpp src/dataloader.cpp include/dataloader.hpp)
