if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options("-pipe" "-g" "-Wall" "-Wextra" "-Wpedantic")
add_compile_options("-mavx")

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
if (CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions("-DNDEBUG")
endif()
