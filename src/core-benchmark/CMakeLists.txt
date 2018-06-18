project(enjector-core-benchmark)

set(SOURCE_FILES
	main.c
	async_benchmarks.h
	async_benchmarks.c
	graph_benchmarks.h
	graph_benchmarks.c
	json_benchmarks.h
	json_benchmarks.c
)

add_executable (enjector-core-benchmark ${SOURCE_FILES})
target_include_directories (enjector-core-benchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../../include)

target_link_libraries (enjector-core-benchmark LINK_PUBLIC enjector-core)
