/*
* test.h
*
* Copyright 2004-2019 Enjector Software, Ltd.
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

#pragma once

#include <stdbool.h>

typedef struct test_t {
    const char* name;
    void(*test_function)();
} test;

typedef struct test_suite_t {
    const char* name;
    test* tests;
} test_suite;

#define TEST_END	   0
#define TEST_SUITE_END 0

bool test_run(test_suite fixtures[]);

void test_assert_equal_bool(bool expected, bool actual, const char* file, int line, bool fatal);
void test_assert_not_equal_bool(bool not_expected, bool actual, const char* file, int line, bool fatal);

void test_assert_equal_int(int expected, int actual, const char* file, int line, bool fatal);
void test_assert_not_equal_int(int not_expected, int actual, const char* file, int line, bool fatal);

void test_assert_equal_long(long expected, long actual, const char* file, int line, bool fatal);
void test_assert_not_equal_long(long not_expected, long actual, const char* file, int line, bool fatal);

void test_assert_equal_string(const char* expected, const char* actual, const char* file, int line, bool fatal);
void test_assert_not_equal_string(const char* not_expected, const char* actual, const char* file, int line, bool fatal);

void test_assert_equal_ptr(void* expected, void* actual, const char* file, int line, bool fatal);
void test_assert_not_equal_ptr(void* not_expected, void* actual, const char* file, int line, bool fatal);

void test_assert_null(void* actual, const char* file, int line, bool fatal);
void test_assert_not_null(void* actual, const char* file, int line, bool fatal);

#define TEST_ASSERT_EQUAL_BOOL(expected, actual) test_assert_equal_bool(expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_EQUAL_BOOL_FATAL(expected, actual) test_assert_equal_bool(expected, actual, __FILE__, __LINE__, true)
#define TEST_ASSERT_NOT_EQUAL_BOOL(not_expected, actual) test_assert_not_equal_bool(not_expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_NOT_EQUAL_BOOL_FATAL(not_expected, actual) test_assert_not_equal_bool(not_expected, actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_EQUAL_INT(expected, actual) test_assert_equal_int(expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_EQUAL_INT_FATAL(expected, actual) test_assert_equal_int(expected, actual, __FILE__, __LINE__, true)
#define TEST_ASSERT_NOT_EQUAL_INT(not_expected, actual) test_assert_not_equal_int(not_expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_NOT_EQUAL_INT_FATAL(not_expected, actual) test_assert_not_equal_int(not_expected, actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_EQUAL_LONG(expected, actual) test_assert_equal_long(expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_EQUAL_LONG_FATAL(expected, actual) test_assert_equal_long(expected, actual, __FILE__, __LINE__, true)
#define TEST_ASSERT_NOT_EQUAL_LONG(not_expected, actual) test_assert_not_equal_long(not_expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_NOT_EQUAL_LONG_FATAL(not_expected, actual) test_assert_not_equal_long(not_expected, actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) test_assert_equal_string(expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_EQUAL_STRING_FATAL(expected, actual) test_assert_equal_string(expected, actual, __FILE__, __LINE__, true)
#define TEST_ASSERT_NOT_EQUAL_STRING(not_expected, actual) test_assert_not_equal_string(not_expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_NOT_EQUAL_STRING_FATAL(not_expected, actual) test_assert_not_equal_string(not_expected, actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_EQUAL_PTR(expected, actual) test_assert_equal_ptr(expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_EQUAL_PTR_FATAL(expected, actual) test_assert_equal_ptr(expected, actual, __FILE__, __LINE__, true)
#define TEST_ASSERT_NOT_EQUAL_PTR(not_expected, actual) test_assert_not_equal_ptr(not_expected, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_NOT_EQUAL_PTR_FATAL(not_expected, actual) test_assert_not_equal_ptr(not_expected, actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_PTR_NULL(actual) test_assert_null((void*) actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_PTR_NULL_FATAL(actual) test_assert_null((void*) actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_PTR_NOT_NULL(actual) test_assert_not_null((void*) actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_PTR_NOT_NULL_FATAL(actual) test_assert_not_null((void*) actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_TRUE(actual) test_assert_equal_bool(true, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_TRUE_FATAL(actual) test_assert_equal_bool(true, actual, __FILE__, __LINE__, true)

#define TEST_ASSERT_FALSE(actual) test_assert_equal_bool(false, actual, __FILE__, __LINE__, false)
#define TEST_ASSERT_FALSE_FATAL(actual) test_assert_equal_bool(false, actual, __FILE__, __LINE__, true)
