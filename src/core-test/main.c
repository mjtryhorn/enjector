/*
* main.c
*
* Copyright 2004-2018 Enjector Software, Ltd.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <enjector/core/test.h>

#include "test_tests.h"
#include "xmemory_tests.h"
#include "clock_tests.h"
#include "clock_stopwatch_tests.h"
#include "clock_timer_tests.h"
#include "list_tests.h"
#include "map_tests.h"
#include "graph_tests.h"
#include "promise_tests.h"
#include "string_buffer_tests.h"
#include "text_tests.h"
#include "json_tests.h"
#include "async_tests.h"
#include "hex_tests.h"
#include "byte_array_tests.h"
#include "base64_tests.h"

#include <stdio.h>
#include <stdbool.h>

test_suite tests[] = {
    { "test_tests", test_tests },

#ifdef _DEBUG_MEMORY
    { "xmemory_tests", xmemory_tests },
#endif

    { "clock_tests", clock_tests },
    { "clock_stopwatch_tests", clock_stopwatch_tests },
    { "clock_timer_tests", clock_timer_tests },
    { "list_tests", list_tests },
    { "map_tests", map_tests },
    { "graph_tests", graph_tests },
    { "string_buffer_tests", string_buffer_tests },
    { "text_tests", text_tests },
    { "promise_tests", promise_tests },
    { "json_tests", json_tests },
    { "async_tests", async_tests },
    { "hex_tests", hex_tests },
    { "byte_array_tests", byte_array_tests },
    { "base64_tests", base64_tests },

    TEST_SUITE_END
};

int main(int argc, const char* argv[]) {
    bool result = test_run(tests);

#ifdef _WIN32 // For running via Visual Studio
    printf("End, press key to close\n");
    getchar();
#endif

    // Return 0 for success and 1 fo failure
    return result ? 0 : 1;
}
