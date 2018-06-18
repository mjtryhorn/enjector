project(enjector-core-test)

set(SOURCE_FILES
	main.c

	async_tests.h
	async_tests.c

	clock_stopwatch_tests.h
	clock_stopwatch_tests.c

	clock_tests.h
	clock_tests.c

	clock_timer_tests.h
	clock_timer_tests.c

	graph_tests.h
	graph_tests.c

	json_tests.h
	json_tests.c

	list_tests.h
	list_tests.c

	map_tests.h
	map_tests.c

	promise_tests.h
	promise_tests.c

	string_buffer_tests.h
	string_buffer_tests.c

	test_tests.h
	test_tests.c

	text_tests.h
	text_tests.c

	xmemory_tests.h
	xmemory_tests.c
)

add_executable (enjector-core-test ${SOURCE_FILES})
target_include_directories (enjector-core-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../../include)

target_link_libraries (enjector-core-test LINK_PUBLIC enjector-core)
