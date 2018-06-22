
#include <stdio.h>

#include "verb.h"
#include "async.h"

#include "benchmark.h"
#include <verb_async.h>

static promise(promise_do_nothing) {
    promise_complete(0);
};

static async(async_do_echo) {
    async_begin {
        await_promise(promise_do_nothing, NULL);
    }
    async_end;
}

int benchmark_simple_verb_async_call_single_param(int argc, const char * argv[]) {

    int count = 5000000;
    int n = count;

    const char* response = NULL;
    int result;

    async_task_scheduler_init(true);
    verb_async_context* context = verb_async_context_current_get();

    result = verb_async_register(context, "echo1", "message", async_do_echo);

    if (result != VERB_RESULT_OK) return BENCHMARK_FAILED;

    while (n-- > 0) {
        async_future* future = verb_async_exec(context, "echo1 hello", NULL);

        if (future == NULL) {
            return BENCHMARK_FAILED;
        }

        async_future_wait(future);
        async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());

        async_future_dispose(future);
    }

    async_task_scheduler_deinit();

    return count;
}

benchmark verb_async_benchmarks[] = {
    { "benchmark_simple_verb_async_call_single_param", benchmark_simple_verb_async_call_single_param },
    BENCHMARK_END,
};
