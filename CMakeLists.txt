cmake_minimum_required (VERSION 3.5.1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(EXECUTABLE_OUTPUT_PATH ../../)

add_subdirectory (src/core)
add_subdirectory (src/core-benchmark)
add_subdirectory (src/core-test)
add_subdirectory (src/main)

