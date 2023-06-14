set(BENCHMARK_ENABLE_TESTING NO)

include(FetchContent)

FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        TIMEOUT 10
        GIT_TAG v1.8.0
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/googlebenchmark
)

FetchContent_MakeAvailable(googlebenchmark)
