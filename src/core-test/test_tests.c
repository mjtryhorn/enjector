/*
* test_tests.c
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

#include <enjector/core/test.h>

#include <stdbool.h>
#include <stddef.h>

void should_successfully_test_equality_bool() {
    bool actual = true;
    TEST_ASSERT_EQUAL_BOOL(true, actual);
    TEST_ASSERT_EQUAL_BOOL_FATAL(true, actual);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE_FATAL(actual);
}

void should_successfully_test_inequality_bool() {
    bool actual = false;
    TEST_ASSERT_NOT_EQUAL_BOOL(true, actual);
    TEST_ASSERT_NOT_EQUAL_BOOL_FATAL(true, actual);
    TEST_ASSERT_FALSE(actual);
    TEST_ASSERT_FALSE_FATAL(actual);
}

void should_successfully_test_equality_int() {
    int actual = 33;
    TEST_ASSERT_EQUAL_INT(33, actual);
    TEST_ASSERT_EQUAL_INT_FATAL(33, actual);
}

void should_successfully_test_inequality_int() {
    int actual = 31;
    TEST_ASSERT_NOT_EQUAL_INT(32, actual);
    TEST_ASSERT_NOT_EQUAL_INT_FATAL(32, actual);
}

void should_successfully_test_equality_long() {
    long actual = 326372678L;
    TEST_ASSERT_EQUAL_LONG(326372678L, actual);
    TEST_ASSERT_EQUAL_LONG_FATAL(326372678L, actual);
}

void should_successfully_test_inequality_long() {
    long actual = 126376787L;
    TEST_ASSERT_NOT_EQUAL_LONG(926372687L, actual);
    TEST_ASSERT_NOT_EQUAL_LONG_FATAL(923726787L, actual);
}

void should_successfully_test_equality_float() {
    float actual = 1.23222111f;
    TEST_ASSERT_EQUAL_FLOAT(1.23222111f, actual);
    TEST_ASSERT_EQUAL_FLOAT_FATAL(1.23222111f, actual);
}

void should_successfully_test_inequality_float() {
    float actual = -221212.2222f;
    TEST_ASSERT_NOT_EQUAL_FLOAT(1.2f, actual);
    TEST_ASSERT_NOT_EQUAL_FLOAT_FATAL(1.2f, actual);
}

void should_successfully_test_equality_string() {
    const char* actual = "Hello World";
    TEST_ASSERT_EQUAL_STRING("Hello World", actual);
    TEST_ASSERT_EQUAL_STRING_FATAL("Hello World", actual);
}

void should_successfully_test_inequality_string() {
    const char* actual = "Hello World";
    TEST_ASSERT_NOT_EQUAL_STRING("Goodbye", actual);
    TEST_ASSERT_NOT_EQUAL_STRING_FATAL("Goodbye", actual);
}

void should_successfully_test_null_ptr() {
    void* actual = NULL;
    TEST_ASSERT_PTR_NULL(actual);
    TEST_ASSERT_PTR_NULL_FATAL(actual);
}

void should_successfully_test_not_null_ptr() {
    void* actual = "Hello";
    TEST_ASSERT_PTR_NOT_NULL(actual);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(actual);
}

test test_tests[] = {
    {"should_successfully_test_equality_bool", should_successfully_test_equality_bool},
    {"should_successfully_test_inequality_bool", should_successfully_test_inequality_bool},
    {"should_successfully_test_equality_int", should_successfully_test_equality_int},
    {"should_successfully_test_inequality_int", should_successfully_test_inequality_int},
    { "should_successfully_test_equality_long", should_successfully_test_inequality_long },
    { "should_successfully_test_inequality_long", should_successfully_test_inequality_long },
    { "should_successfully_test_equality_float", should_successfully_test_equality_float },
    { "should_successfully_test_inequality_float", should_successfully_test_inequality_float },
    { "should_successfully_test_equality_string", should_successfully_test_equality_string },
    { "should_successfully_test_equality_string", should_successfully_test_inequality_string },
    { "should_successfully_test_inequality_string", should_successfully_test_inequality_string },
    { "should_successfully_test_null_ptr", should_successfully_test_null_ptr },
    { "should_successfully_test_not_null_ptr", should_successfully_test_not_null_ptr },
    TEST_END
};
