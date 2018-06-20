/*
* clock_stopwatch_tests.c
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

#include "clock_tests.h"

#include <enjector/core/clock_stopwatch.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/system_sleep.h>
#include <enjector/core/clock.h>

#include <stdbool.h>

static void should_successfully_test_stop_watch_start_end() {
    clock_stopwatch stopwatch;
    clock_stopwatch_start(&stopwatch);

    system_sleep(100);

    clock_time_quantity v1 = clock_stopwatch_end(&stopwatch);
    clock_time_quantity v2 = clock_stopwatch_elapsed(&stopwatch);
    TEST_ASSERT_EQUAL_SIZE(v1, v2);
    TEST_ASSERT_TRUE(v1 >= 100);

    xmemory_report_exit_on_leaks();
}


static void should_successfully_test_stop_watch_start_end_continue() {
    clock_stopwatch stopwatch;
    clock_stopwatch_start(&stopwatch);

    system_sleep(100);

    clock_stopwatch_end(&stopwatch);
    clock_stopwatch_start(&stopwatch);

    system_sleep(200);

    clock_time_quantity v1 = clock_stopwatch_end(&stopwatch);
    TEST_ASSERT_TRUE(v1 >= 200);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_test_rate_count() {
    clock_stopwatch stopwatch;
    clock_stopwatch_start(&stopwatch);

    system_sleep(1000);

    clock_stopwatch_elapsed(&stopwatch);

    const size_t rate = clock_stopwatch_rate_calculate(&stopwatch, 5000);
    TEST_ASSERT_TRUE(rate >= 1);

    xmemory_report_exit_on_leaks();
}

test clock_stopwatch_tests[] = {
    { "should_successfully_test_stop_watch_start_end", should_successfully_test_stop_watch_start_end },
    { "should_successfully_test_stop_watch_start_end_continue", should_successfully_test_stop_watch_start_end_continue },
    { "should_successfully_test_rate_count", should_successfully_test_rate_count },
    TEST_END
};
