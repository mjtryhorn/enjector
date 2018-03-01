/*
* text_tests.c
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

#include "test_tests.h"

#include <enjector/core/text.h>
#include <enjector/core/xmemory.h>

static void should_successfully_compare_same_case_equal() {
    const char* string1 = "ABCDEF";
    const char* string2 = "ABCDEF";
    int expected_result = 1;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_nocase_equal() {
    const char* string1 = "abcdef";
    const char* string2 = "ABCDEF";
    int expected_result = 1;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_nocase_not_equal_partially() {
    const char* string1 = "abcdeA";
    const char* string2 = "ABCDEF";
    int expected_result = -5;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_nocase_not_equal() {
    const char* string1 = "AAAA^&^&*AGHJ";
    const char* string2 = "ABCDEF";
    int expected_result = -1;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_nocase_partially_left() {
    const char* string1 = "abc";
    const char* string2 = "ABCDEF";
    int expected_result = -100;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_nocase_partially_right() {
    const char* string1 = "def";
    const char* string2 = "ABCDEF";
    int expected_result = 3;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_clone() {
    const char* string1 = "def1234";
    char* string2 = text_clone(string1);

    int expected_result = 1;

    int actual_result = text_compare_nocase(string1, string2);

    TEST_ASSERT_EQUAL_INT(expected_result, actual_result);

    text_free(string2);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_split_text() {
    const char* str = "A\nB\nC";

    list* rows = text_split(str, '\n');
    TEST_ASSERT_PTR_NOT_NULL(rows);
    TEST_ASSERT_EQUAL_INT_FATAL(3, list_count(rows));

    TEST_ASSERT_EQUAL_STRING("A", list_get_item(rows, 0)->value);
    TEST_ASSERT_EQUAL_STRING("B", list_get_item(rows, 1)->value);
    TEST_ASSERT_EQUAL_STRING("C", list_get_item(rows, 2)->value);

    list_dispose(rows);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_replace_text1() {
    const char* str = "AhelloC";
    const char* expected = "AbyeC";

    char* actual = text_replace(str, "hello", "bye");
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_replace_text2() {
    const char* str = "AhelloC";
    const char* expected = "ahelloC";

    char* actual = text_replace(str, "A", "a");
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_replace_text3() {
    const char* str = "AhelloC";
    const char* expected = "Ahellzc";

    char* actual = text_replace(str, "oC", "zc");
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_replace_text4() {
    const char* str = "zz zz";
    const char* expected = "z z";

    char* actual = text_replace(str, "zz", "z");
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_replace_text5() {
    const char* str = "zz\"\"z z\"\"";
    const char* expected = "zz\"z z\"";

    char* actual = text_replace(str, "\"\"", "\"");
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_replace_text6() {
    const char* str = "{\"\"establishedConnection\"\":{\"\"callID\"\":\"\"5912\"\",\"\"devIDType\"\":\"\"staticId\"\",\"\"deviceID\"\":\"\"3181689\"\"},\"\"lastRedirectionDevice\"\":{\"\"deviceIDType\"\":\"\"explicitPublicUnknown\"\",\"\"deviceIDStatus\"\":\"\"idNotRequired\"\",\"\"deviceID\"\":\"\"\"\"},\"\"localConnectionInfo\"\":\"\"csNull\"\",\"\"calledDevice\"\":{\"\"deviceIDType\"\":\"\"explicitPrivateLocalNumber\"\",\"\"deviceIDStatus\"\":\"\"idProvided\"\",\"\"deviceID\"\":\"\"3181689\"\"},\"\"answeringDevice\"\":{\"\"deviceIDType\"\":\"\"explicitPrivateLocalNumber\"\",\"\"deviceIDStatus\"\":\"\"idProvided\"\",\"\"deviceID\"\":\"\"1181689\"\"},\"\"cause\"\":\"\"ecNewCall\"\",\"\"callingDevice\"\":{\"\"deviceIDType\"\":\"\"explicitPublicUnknown\"\",\"\"deviceIDStatus\"\":\"\"idProvided\"\",\"\"deviceID\"\":\"\"442011111111\"\"}}";
    const char* expected = "{\"establishedConnection\":{\"callID\":\"5912\",\"devIDType\":\"staticId\",\"deviceID\":\"3181689\"},\"lastRedirectionDevice\":{\"deviceIDType\":\"explicitPublicUnknown\",\"deviceIDStatus\":\"idNotRequired\",\"deviceID\":\"\"},\"localConnectionInfo\":\"csNull\",\"calledDevice\":{\"deviceIDType\":\"explicitPrivateLocalNumber\",\"deviceIDStatus\":\"idProvided\",\"deviceID\":\"3181689\"},\"answeringDevice\":{\"deviceIDType\":\"explicitPrivateLocalNumber\",\"deviceIDStatus\":\"idProvided\",\"deviceID\":\"1181689\"},\"cause\":\"ecNewCall\",\"callingDevice\":{\"deviceIDType\":\"explicitPublicUnknown\",\"deviceIDStatus\":\"idProvided\",\"deviceID\":\"442011111111\"}}";

    char* actual = text_replace(str, "\"\"", "\"");
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_trim_left() {
    const char* str = "  Hello";
    const char* expected = "Hello";

    char* actual = text_trim_left(str);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    xmemory_free(actual);

    // Check trim doesn't happen
    actual = text_trim_left(expected);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_trim_right() {
    const char* str = "Hello  ";
    const char* expected = "Hello";

    char* actual = text_trim_right(str);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    xmemory_free(actual);

    // Check trim doesn't happen
    actual = text_trim_right(expected);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_trim() {
    const char* str = "  Hello   ";
    const char* expected = "Hello";

    char* actual = text_trim(str);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    xmemory_free(actual);

    // Check trim doesn't happen
    actual = text_trim(expected);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    xmemory_free(actual);

    xmemory_report_exit_on_leaks();
}

test text_tests[] = {
    { "should_successfully_compare_same_case_equal", should_successfully_compare_same_case_equal },
    { "should_successfully_compare_nocase_equal", should_successfully_compare_nocase_equal },
    { "should_successfully_compare_nocase_not_equal_partially", should_successfully_compare_nocase_not_equal_partially },
    { "should_successfully_compare_nocase_not_equal", should_successfully_compare_nocase_not_equal },
    { "should_successfully_compare_nocase_partially_left", should_successfully_compare_nocase_partially_left },
    { "should_successfully_compare_nocase_partially_right", should_successfully_compare_nocase_partially_right },
    { "should_successfully_clone", should_successfully_clone },
    { "should_successfully_split_text", should_successfully_split_text },
    { "should_successfully_replace_text1", should_successfully_replace_text1 },
    { "should_successfully_replace_text2", should_successfully_replace_text2 },
    { "should_successfully_replace_text3", should_successfully_replace_text3 },
    { "should_successfully_replace_text4", should_successfully_replace_text4 },
    { "should_successfully_replace_text5", should_successfully_replace_text5 },
    { "should_successfully_replace_text6", should_successfully_replace_text6 },
    { "should_successfully_trim_left", should_successfully_trim_left },
    { "should_successfully_trim_right", should_successfully_trim_right },
    { "should_successfully_trim", should_successfully_trim },
    TEST_END
};
