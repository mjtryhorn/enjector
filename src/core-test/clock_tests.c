/*
* clock_tests.c
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

#include <enjector/core/clock.h>
#include <enjector/core/xmemory.h>

static void should_successfully_get_now_milliseconds() {
    const long actual_result = clock_now_milliseconds();

    TEST_ASSERT_NOT_EQUAL_INT(0, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_get_now_seconds() {
    const long actual_result = clock_now_seconds();

    TEST_ASSERT_NOT_EQUAL_INT(0, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_get_now_utc() {
    const char* timestamp = clock_now_utc();

    TEST_ASSERT_PTR_NOT_NULL(timestamp);

    xmemory_report_exit_on_leaks();
}

test clock_tests[] = {
    { "should_successfully_get_now_milliseconds", should_successfully_get_now_milliseconds },
    { "should_successfully_get_now_seconds", should_successfully_get_now_seconds },
    { "should_successfully_get_now_utc", should_successfully_get_now_utc },
    TEST_END
};
