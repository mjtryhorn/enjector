/*
* async_benchmarks.c
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

#include "async_benchmarks.h"

#include <enjector/core/promise.h>
#include <enjector/core/async.h>

static promise(promise_do_nothing) {
    promise_complete(0);
};

static async(async_do_echo) {
    async_begin {
        await_promise(promise_do_nothing, NULL);
    }
    async_end;
}

int benchmark_simple_async(int argc, const char * argv[]) {

    int count = 5000000;
    int n = count;

    async_task_scheduler_init(false);

    while (n-- > 0) {
        async_task* t = go(async_do_echo, NULL);

        if (t == NULL) {
            return BENCHMARK_FAILED;
        }

        async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());

        async_task_free(t);
    }

    async_task_scheduler_deinit();

    return count;
}

int benchmark_simple_async_with_auto_destroy(int argc, const char * argv[]) {
    int count = 5000000;
    int n = count;

    async_task_scheduler_init(true);

    while (n-- > 0) {
        go(async_do_echo, NULL);
        async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());
    }

    async_task_scheduler_deinit();

    return count;
}

benchmark async_benchmarks[] = {
    { "benchmark_simple_async", benchmark_simple_async },
    { "benchmark_simple_async_with_auto_destroy", benchmark_simple_async_with_auto_destroy },
    BENCHMARK_END,
};
