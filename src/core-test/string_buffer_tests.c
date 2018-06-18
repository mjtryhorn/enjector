/*
* promise_tests.c
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

#include "string_buffer_tests.h"

#include <enjector/core/string_buffer.h>
#include <enjector/core/xmemory.h>

#include <assert.h>
#include <string.h>

static void should_successfully_create_string_buffer_and_append_some_items() {
    string_buffer* sb = string_buffer_create_empty();
    assert(sb);

    const char* expected_value = "Hello World";

    string_buffer_append(sb, "Hello");
    string_buffer_append(sb, " World");

    TEST_ASSERT_EQUAL_SIZE(string_buffer_length(sb), (int) strlen(expected_value));
    TEST_ASSERT_EQUAL_STRING(expected_value, string_buffer_get(sb));

    string_buffer_free(sb);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_string_buffer_and_append_format_some_items() {
    string_buffer* sb = string_buffer_create_empty();
    assert(sb);

    const char* expected_value = "Hello Fred Bob World";

    string_buffer_append_format(sb, "Hello %s %s", "Fred", "Bob");
    string_buffer_append(sb, " World");

    TEST_ASSERT_EQUAL_SIZE(string_buffer_length(sb), (int)strlen(expected_value));
    TEST_ASSERT_EQUAL_STRING(expected_value, string_buffer_get(sb));

    string_buffer_free(sb);

    xmemory_report_exit_on_leaks();
}

test string_buffer_tests[] = {
    { "should_successfully_create_string_buffer_and_append_some_items", should_successfully_create_string_buffer_and_append_some_items },
    { "should_successfully_create_string_buffer_and_append_format_some_items", should_successfully_create_string_buffer_and_append_format_some_items },
    TEST_END
};
