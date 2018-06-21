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

file(MAKE_DIRECTORY analysis/coverage/)
file(MAKE_DIRECTORY analysis/static/)
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES analysis/)

add_custom_target(
    coverage
    DEPENDS enjector-core-test
    COMMAND ./enjector-core-test
    COMMAND gcovr -r . --html --html-details -o analysis/coverage/index.html
    COMMAND echo "Report in analysis/coverage/"
)

add_custom_target(
    staticcheck
    cppcheck --enable=all --inconclusive --xml-version=2 --force --library=windows,posix,gnu ./src/ 2> analysis/static/result.xml && cppcheck-htmlreport --source-encoding="iso8859-1" --title="Enjector" --source-dir=./ --report-dir=analysis/static/ --file=analysis/static/result.xml
)

add_custom_target(
    memcheck
    DEPENDS enjector-core-test
    COMMAND valgrind --show-leak-kinds=all --leak-check=full ./enjector-core-test
)

