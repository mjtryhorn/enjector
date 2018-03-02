/*
* benchmark.h
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

#pragma once

#include <stdbool.h>

typedef struct benchmark_t {
    const char* name;
    int (*benchmark_function)();
} benchmark;

typedef struct benchmark_suite_t {
    const char* name;
    benchmark* benchmarks;
} benchmark_suite;

#define BENCHMARK_FAILED   -1
#define BENCHMARK_END	    0
#define BENCHMARK_SUITE_END 0

bool benchmark_run(benchmark_suite fixtures[]);
