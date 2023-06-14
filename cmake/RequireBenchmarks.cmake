project(regression_benchmarks)

file(GLOB_RECURSE BENCH_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/*.cpp)

add_executable(${PROJECT_NAME} ${BENCH_SOURCE})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        linear_regression
        benchmark::benchmark
)
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
