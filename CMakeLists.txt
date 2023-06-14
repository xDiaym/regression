cmake_minimum_required(VERSION 3.26)
project(linear_regression)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/SetupCCache.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/SetupLTO.cmake)


add_library(${PROJECT_NAME}
    STATIC
        src/linalg.cpp
        src/regression.cpp
        src/dataloader.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


include(cmake/SetupBenchmarks.cmake)
