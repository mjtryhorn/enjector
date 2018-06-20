/*
* async_benchmarks.h
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

#undef _MSC_VER

#include <enjector/core/benchmark.h>

#include "json_benchmarks.h"
#include "async_benchmarks.h"
#include "graph_benchmarks.h"

#include <stdio.h>

benchmark_suite benchmarks[] = {
    { "json_benchmarks", json_benchmarks },
    { "async_benchmarks", async_benchmarks },
    { "graph_benchmarks", graph_benchmarks },
    BENCHMARK_SUITE_END
};

#ifdef _MSC_VER
#include <windows.h>
#include <tchar.h>
#endif

int main(int argc, const char* argv[]) {

#ifdef _MSC_VER
    HANDLE process = GetCurrentProcess();
    DWORD_PTR processAffinityMask = 1;

    BOOL success = SetProcessAffinityMask(process, processAffinityMask);

    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL)) {
        _tprintf(TEXT("Failed to enter THREAD_PRIORITY_TIME_CRITICAL mode\n"));
        return 0;
    }

#endif

    bool result = benchmark_run(benchmarks);

#ifdef _MSC_VER

    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL)) {
        _tprintf(TEXT("Failed to enter THREAD_PRIORITY_BELOW_NORMAL mode\n"));
    }

#endif

//#if _DEBUG
    printf("End, press key to close\n");
    getchar();
//#endif

    // Return 0 for success and 1 fo failure
    return result ? 0 : 1;
}
