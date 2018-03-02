/*
* benchmark.c
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

#include <enjector/core/benchmark.h>

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "enjector/core/clock_stopwatch.h"

void _benchmark_exit_on_fatal(bool fatal) {
    if (fatal) {
        exit(1);
    }
}

bool benchmark_run(benchmark_suite fixtures[]) {
    assert(fixtures != NULL);

    bool all_passed = true;

    int i = 0;
    benchmark_suite *fixture;

    while (fixture = &fixtures[i++], fixture->name != BENCHMARK_SUITE_END) {
        assert(fixture);

        printf("Benchmark: %s\n", fixture->name);
        int j = 0;

        benchmark *benchmark;

        while (benchmark = &fixture->benchmarks[j++], benchmark->name != BENCHMARK_END) {
            assert(benchmark);

            clock_stopwatch s;
            clock_stopwatch_start(&s);

            int count = benchmark->benchmark_function();
            if (count != BENCHMARK_FAILED) {
                clock_stopwatch_end(&s);
                double rate = clock_stopwatch_rate_calculate(&s, count);
                printf("\tSUCCESS: %s: %f per second (count:=%d)\n", benchmark->name, rate, count);
            } else {
                printf("\tFAILED : %s\n", benchmark->name);
                all_passed = false;
            }
        }

        printf("\n");
    }

    // TODO: show summary with counters
    if (all_passed) {
        printf("All benchmark tests succeeded\n");
    } else {
        printf("Some benchmark tests failed\n");
    }

	return all_passed;
}

