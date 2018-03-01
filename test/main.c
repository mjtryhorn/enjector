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

#include "test_tests.h"
#include "xmemory_tests.h"
#include "clock_tests.h"
#include "clock_stopwatch_tests.h"
#include "clock_timer_tests.h"
#include "promise_tests.h"

test_suite tests[] = {
    { "test_tests", test_tests },
    { "clock_tests", clock_tests },
    { "clock_stopwatch_tests", clock_stopwatch_tests },
    { "clock_timer_tests", clock_timer_tests },
    { "promise_tests", promise_tests },

#ifdef _DEBUG
    { "xmemory_tests", xmemory_tests},
#endif

    TEST_SUITE_END
};

int main(int argc, const char* argv[]) {
    // Return 0 for success and 1 fo failure
    return test_run(tests) ? 0 : 1;
}
