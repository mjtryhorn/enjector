/*
* queue_tests.c
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

#include "queue_tests.h"

#include <enjector/core/queue.h>
#include <enjector/core/xmemory.h>

#include <string.h>

static void should_successfully_have_no_messages_when_no_messages_are_queued() {
    queue* queue = queue_create();

    unsigned int count = queue_count(queue);
    TEST_ASSERT_EQUAL_INT(0, count);

    queue_free(queue);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_enqueue_deque_single_item() {
    queue* queue = queue_create();

    const char* data_expected = "00A00000123456";
    queue_push(queue, (void*) data_expected);
    TEST_ASSERT_EQUAL_INT(1, queue_count(queue));

    char* data_actual = queue_pop(queue);
    TEST_ASSERT_TRUE(!strcmp(data_actual, data_expected));

    queue_free(queue);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_enqueue_deque_multiple_items() {
    queue* queue = queue_create();

    const char* data_expected1 = "00A00000123456";
    const char* data_expected2 = "AABBCCC00718274428";

    queue_push(queue, (void*) data_expected1);
    queue_push(queue, (void*) data_expected2);
    TEST_ASSERT_EQUAL_INT(2, queue_count(queue));

    char* data_actual1 = queue_pop(queue);
    char* data_actual2 = queue_pop(queue);
    TEST_ASSERT_TRUE(!strcmp(data_actual1, data_expected2));
    TEST_ASSERT_TRUE(!strcmp(data_actual2, data_expected1));

    queue_free(queue);

    xmemory_report_exit_on_leaks();
}

test queue_tests[] = {
    { "should_successfully_have_no_messages_when_no_messages_are_queued", should_successfully_have_no_messages_when_no_messages_are_queued },
    { "should_successfully_enqueue_deque_single_item", should_successfully_enqueue_deque_single_item },
    { "should_successfully_enqueue_deque_multiple_items", should_successfully_enqueue_deque_multiple_items },
    { "should_successfully_enqueue_deque_multiple_items", should_successfully_enqueue_deque_multiple_items },
    TEST_END
};
