project(enjector)

add_executable (enjector "main.c")
target_include_directories (enjector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../../include)

target_link_libraries (enjector LINK_PUBLIC enjector-core)
