/*
* test.c
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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef _MSC_VER
#include <consoleapi.h>
#endif

void _test_console_set_foreground_yellow() {
#ifdef _MSC_VER
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, 0x0E);
#endif
}

void _test_console_set_foreground_green() {
#ifdef _MSC_VER
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, 0x0A);
#endif
}

void _test_console_set_foreground_red() {
#ifdef _MSC_VER
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, 0x0C);
#endif
}

void _test_console_set_foreground_purple() {
#ifdef _MSC_VER
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, 0x0D);
#endif
}

void _test_console_set_foreground_white() {
#ifdef _MSC_VER
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, 0x0F);
#endif
}

void _test_exit_on_fatal(bool fatal) {
    if (fatal) {
        exit(1);
    }
}

// This flag is reset for every test that is run and tripped by the assertions below
bool _flag_test_failed = false;

bool test_run(test_suite fixtures[]) {

    assert(fixtures != NULL);

    bool all_passed = true;
    unsigned int counter_passed = 0;
    unsigned int counter_failed = 0;

    int i = 0;
    test_suite *fixture;

    while (fixture = &fixtures[i++], fixture->name != TEST_SUITE_END) {
        assert(fixture);

        _test_console_set_foreground_purple();
        printf("Fixture: %s\n", fixture->name);
        _test_console_set_foreground_white();

        int j = 0;
        test *test;

        while (test = &fixture->tests[j++], test->name != TEST_END) {
            assert(test);

            // Reset the error trip, if an error occurs then the flag _flag_test_failed will be true
            _flag_test_failed = false;

            _test_console_set_foreground_yellow();
            printf("\tTest: %s ... ", test->name);
            _test_console_set_foreground_white();
            test->test_function();

            if (_flag_test_failed) {
                _test_console_set_foreground_red();
                printf("FAILED\n");
                all_passed = false;
                counter_failed++;
            } else {
                _test_console_set_foreground_green();
                printf("SUCCESS\n");
                counter_passed++;
            }

            _test_console_set_foreground_white();
        }

        printf("\n");
    }

    if (all_passed) {
        printf("All test passed (passed: %d)\n", counter_passed);
    } else {
        printf("Some tests failed (failed: %d, passed: %d)\n", counter_failed, counter_passed);
    }

    return all_passed;
}

void test_assert_equal_bool(bool expected, bool actual, const char* file, int line, bool fatal) {
    if (expected != actual) {
        printf("\n\tExpected '%d' to equal '%d'\n\t\t%s:%d\n", expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_not_equal_bool(bool not_expected, bool actual, const char* file, int line, bool fatal) {
    if (not_expected == actual) {
        printf("\n\tExpected '%d' to not_equal '%d'\n\t\t%s:%d\n", not_expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_equal_int(int expected, int actual, const char* file, int line, bool fatal) {
    if (expected != actual) {
        printf("\n\tExpected '%d' to equal '%d'\n\t\t%s:%d\n", expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_not_equal_int(int not_expected, int actual, const char* file, int line, bool fatal) {
    if (not_expected == actual) {
        printf("\n\tExpected '%d' to not_equal '%d'\n\t\t%s:%d\n", not_expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_equal_long(long expected, long actual, const char* file, int line, bool fatal) {
    if (expected != actual) {
        printf("\n\tExpected '%ld' to equal '%ld'\n\t\t%s:%d\n", expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_not_equal_long(long not_expected, long actual, const char* file, int line, bool fatal) {
    if (not_expected == actual) {
        printf("\n\tExpected '%ld' to not_equal '%ld'\n\t\t%s:%d\n", not_expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_equal_string(const char* expected, const char* actual, const char* file, int line, bool fatal) {
    if (expected != actual || !strcmp(expected, actual)) {
        printf("\n\tExpected '%s' to equal '%s'\n\t\t%s:%d\n", expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_not_equal_string(const char* not_expected, const char* actual, const char* file, int line, bool fatal) {
    if (not_expected == actual || !strcmp(not_expected, actual)) {
        printf("\n\tExpected '%s' to not_equal '%s'\n\t\t%s:%d\n", not_expected, actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_equal_ptr(void* expected, void* actual, const char* file, int line, bool fatal) {
    if (expected != actual) {
        printf("\n\tExpected '%ld' to equal '%ld'\n\t\t%s:%d\n", (long)expected, (long)actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_not_equal_ptr(void* not_expected, void* actual, const char* file, int line, bool fatal) {
    if (not_expected == actual) {
        printf("\n\tExpected '%ld' to not_equal '%ld'\n\t\t%s:%d\n", (long) not_expected, (long)actual, file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_null(void* actual, const char* file, int line, bool fatal) {
    if (actual != NULL) {
        printf("\n\tExpected to be null\n\t\t%s:%d\n", file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}

void test_assert_not_null(void* actual, const char* file, int line, bool fatal) {
    if (actual == NULL) {
        printf("\tExpected not be null\n\t\t%s:%d\n", file, line);
        _test_exit_on_fatal(fatal);
        _flag_test_failed = true;
    }
}
