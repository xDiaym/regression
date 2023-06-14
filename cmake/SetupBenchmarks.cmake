option(REGRESSION_BUILD_BENCHMARKS "Build benchmarks" TRUE)

if (REGRESSION_BUILD_BENCHMARKS)
    message(STATUS "Benchmarks: enabled")
    include(cmake/SetupGBench.cmake)
    include(cmake/RequireBenchmarks.cmake)
else ()
    message(STATUS "Benchmarks: disabled")
endif ()
