cmake_minimum_required(VERSION 3.26)
project(linear_regression)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_EXECUTABLE ccache)
if (CCACHE_EXECUTABLE)
    message(STATUS "ccache: enabled")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
endif ()

include(cmake/CompilerOptions.cmake)
include(cmake/SetupGBench.cmake)

include_directories(include)
add_executable(linear_regression main.cpp src/linalg.cpp src/regression.cpp src/dataloader.cpp)
target_link_libraries(linear_regression benchmark::benchmark)
