
#include <stdio.h>

#include "verb.h"

#include "benchmark.h"


static verb(echo) {
    char* message = verb_arg("message");

    if (message) {
//		printf("Got a echo message: %s", message);
        return message;
    }

    return NULL;
}

int benchmark_simple_verb_call_single_param(int argc, const char * argv[]) {

    int count = 5000000;
    int n = count;

    const char* response = NULL;
    int result;

    verb_context* context = verb_context_create();

    result = verb_register(context, "echo", "message", echo, NULL);

    if (result != VERB_RESULT_OK) return BENCHMARK_FAILED;

    while(n-->0) {
        int result = verb_exec(context, "echo hello", &response);

        if (result != VERB_RESULT_OK) {
            return BENCHMARK_FAILED;

        }
    }

    verb_context_dispose(context);

    return count;
}

benchmark verb_benchmarks[] = {
    { "benchmark_simple_verb_call_single_param", benchmark_simple_verb_call_single_param },
    BENCHMARK_END,
};
