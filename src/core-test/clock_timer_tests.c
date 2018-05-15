/*
* clock_timer_tests.c
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

#include "clock_timer_tests.h"

#include <enjector/core/clock_timer.h>
#include <enjector/core/sleep.h>
#include <enjector/core/xmemory.h>

static void should_successfully_test_timer_not_expired() {
    clock_timer timer;
    clock_timer_init(&timer, 100);

    sleep(50);
    TEST_ASSERT_FALSE(clock_timer_has_expired(&timer));

    const clock_time_quantity v = clock_timer_value(&timer);
    TEST_ASSERT_TRUE(v > 40);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_test_timer_expired() {
    clock_timer timer;
    clock_timer_init(&timer, 50);

    sleep(60);
    TEST_ASSERT_TRUE(clock_timer_has_expired(&timer));

    xmemory_report_exit_on_leaks();
}

static void should_successfully_test_timer_value() {
    clock_timer timer;
    clock_timer_init(&timer, 50);

    TEST_ASSERT_TRUE(clock_timer_value(&timer) > 40);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_test_timer_reset() {
    clock_timer timer;
    clock_timer_init(&timer, 100);

    sleep(50);
    TEST_ASSERT_FALSE(clock_timer_has_expired(&timer));

    const clock_time_quantity v = clock_timer_value(&timer);
    TEST_ASSERT_TRUE(v > 40);

    xmemory_report_exit_on_leaks();
}

test clock_timer_tests[] = {
    { "should_successfully_test_timer_not_expired", should_successfully_test_timer_not_expired },
    { "should_successfully_test_timer_expired", should_successfully_test_timer_expired },
    { "should_successfully_test_timer_value", should_successfully_test_timer_value },
    { "should_successfully_test_timer_reset", should_successfully_test_timer_reset },
    TEST_END
};
