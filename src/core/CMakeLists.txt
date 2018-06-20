project(enjector-core)
set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

set(INCLUDE_PATH_CORE ../../include/enjector/core )

set(SOURCE_FILES
	${INCLUDE_PATH_CORE}/async.h
	async.c

	${INCLUDE_PATH_CORE}/base64.h
	base64.c

	${INCLUDE_PATH_CORE}/benchmark.h
	benchmark.c

	${INCLUDE_PATH_CORE}/byte_array.h
	byte_array.c

	${INCLUDE_PATH_CORE}/clock_stopwatch.h
	clock_stopwatch.c

	${INCLUDE_PATH_CORE}/clock_timer.h
	clock_timer.c

	${INCLUDE_PATH_CORE}/clock.h
	clock.c

	${INCLUDE_PATH_CORE}/graph.h
	graph.c

	${INCLUDE_PATH_CORE}/hex.h
	hex.c

	${INCLUDE_PATH_CORE}/json.h
	json.c

	${INCLUDE_PATH_CORE}/list.h
	list.c

	${INCLUDE_PATH_CORE}/map.h
	map.c

	${INCLUDE_PATH_CORE}/promise.h
	promise.c

	${INCLUDE_PATH_CORE}/async.h
	async.c

	${INCLUDE_PATH_CORE}/system_sleep.h
	system_sleep_unix.c
	system_sleep_win32.c

	${INCLUDE_PATH_CORE}/string_buffer.h
	string_buffer.c

	${INCLUDE_PATH_CORE}/test.h
	test.c

	${INCLUDE_PATH_CORE}/text.h
	text.c

	${INCLUDE_PATH_CORE}/xmemory.h
	xmemory.c
)

add_library (enjector-core ${SOURCE_FILES})

target_include_directories (enjector-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

