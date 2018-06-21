cmake_minimum_required (VERSION 3.5.1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(WIN32)
  # set stuff for windows
  set(CMAKE_C_FLAGS_WIN32 "-D_WIN32")
endif()

set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG ${CMAKE_C_FLAGS_WIN32} -g -fprofile-arcs -ftest-coverage -fPIC -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall ${CMAKE_C_FLAGS_WIN32}")

set(EXECUTABLE_OUTPUT_PATH "../../")

add_subdirectory (src/core)
add_subdirectory (src/core-benchmark)
add_subdirectory (src/core-test)
add_subdirectory (src/main)

add_custom_target(
    coverage
    COMMAND gcovr -r . --html --html-details -o coverage/enjector-coverage.html
 )

add_custom_target(
    memcheck
    COMMAND valgrind --show-leak-kinds=all --leak-check=full ./enjector-core-test
 )
