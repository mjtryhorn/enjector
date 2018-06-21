/*
* promise_tests.c
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

#include "promise_tests.h"

#include <enjector/core/promise.h>
#include <enjector/core/xmemory.h>

static promise(promise_test_empty) {
}

static void should_sucessfully_set_state_to_READY_when_a_promise_has_been_selected_to_be_executed() {
    promise p;
    promise_init(&p, promise_test_empty, NULL);

    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateReady);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_defer) {
    promise_defer();
}

static void should_sucessfully_set_state_to_DEFER_when_the_promise_calls_defer() {
    promise p;
    promise_init(&p, promise_test_defer, NULL);

    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateReady);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_deferred_counter) {
    static int runs = 0;

    if(runs == 0) {
        runs++;
        promise_defer();
    }
}

static void should_successfuly_set_state_to_RUNNING_when_the_promise_is_running_and_hasnt_fulfilled_or_errored() {
    promise p;

    promise_init(&p, promise_test_empty, NULL);
    promise_run(&p);

    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateRunning);

    xmemory_report_exit_on_leaks();
}

static void should_successfuly_set_state_to_RUNNING_when_a_deferred_promise_is_reexecuted() {
    promise p;
    promise_init(&p, promise_test_deferred_counter, NULL);

    // Should defer for the first run
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateDefer);

    // Run the deferred promise again, it shouldn't get deferred and should be running
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateRunning);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_fulfil) {
    static int runs = 0;

    if(runs == 0) {
        runs++;
        promise_defer();
    } else {
        promise_complete((void*) 2);
    }
}

static void should_successfuly_set_state_to_RESULT_when_a_deferred_promise_is_reexecuted_with_fulfill() {
    promise p;
    promise_init(&p, promise_test_fulfil, NULL);

    // Should defer for the first run
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateDefer);

    // Run the deferred promise again, it shouldn't get deferred and should end up with a result
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateResult);
    TEST_ASSERT_EQUAL_INT(promise_result(p, int), 2);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_defer_then_error) {
    static int runs = 0;

    if(runs == 0) {
        runs++;
        promise_defer();
    } else {
        promise_reject("Defer error");
    }
}

static void should_successfuly_set_state_to_ERROR_when_a_deferred_promise_is_reexecuted_with_reject() {

    promise p;
    promise_init(&p, promise_test_defer_then_error, NULL);

    // Should defer for the first run
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateDefer);

    // Run the deferred promise again, it shouldn't get deferred and should end up with a result
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateError);
    TEST_ASSERT_EQUAL_STRING("Defer error", p.error);

    xmemory_report_exit_on_leaks();
}

static void should_error_when_a_promise_is_reexecuted_when_it_is_in_state_RUNNING_and_not_in_state_READY_or_DEFER() {

    promise p;
    promise_init(&p, promise_test_empty, NULL);

    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateRunning);

    // Run again, shouldn't be allowed. Leave the state as-is
    // TODO: exception: "Cannot execute promise whilst not in READY or DEFER states");
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateRunning);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_reject) {
    promise_reject("An error");
}

static void should_error_when_a_promise_is_reexecuted_when_it_is_in_state_ERROR_and_not_in_state_READY_or_DEFER() {

    promise p;
    promise_init(&p, promise_test_reject, NULL);

    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateError);
    TEST_ASSERT_EQUAL_STRING("An error", p.error);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_fulfil_counter) {
    static int runs = 0;
    promise_complete((void*)++runs);
}

static void should_error_when_a_promise_is_reexecuted_when_it_is_in_state_RESULT_and_not_in_state_READY_or_DEFER() {
    promise p;
    promise_init(&p, promise_test_fulfil_counter, NULL);

    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateResult);
    TEST_ASSERT_EQUAL_INT(promise_result(p, int), 1);

    // Run again, shouldn't be allowed. Leave the state as-is
    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateResult);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_return_arg, long, v) {
    promise_complete(v);
}

static void should_successfuly_set_state_to_RESULT_with_a_passed_in_parameter_when_the_promise_calls_fulfill() {
    void* ctx = { (void*) 1234};
    promise p;
    promise_init(&p, promise_test_return_arg, ctx);

    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateResult);
    TEST_ASSERT_EQUAL_INT(promise_result(p, int), 1234);

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_args, long*, values) {
    promise_complete((void*)(values[0] + values[1]));
}

static void should_successfuly_pass_args_into_promise() {
    long ctx[] = {10, 20};
    promise p;
    promise_init(&p, promise_test_args, ctx);

    promise_run(&p);
    TEST_ASSERT_EQUAL_INT(p.state, PromiseStateResult);
    TEST_ASSERT_EQUAL_INT(promise_result(p, int), 30);

    xmemory_report_exit_on_leaks();
}
test promise_tests[] = {
    { "should_sucessfully_set_state_to_READY_when_a_promise_has_been_selected_to_be_executed", should_sucessfully_set_state_to_READY_when_a_promise_has_been_selected_to_be_executed },
    { "should_sucessfully_set_state_to_DEFER_when_the_promise_calls_defer", should_sucessfully_set_state_to_DEFER_when_the_promise_calls_defer },
    { "should_successfuly_set_state_to_RUNNING_when_the_promise_is_running_and_hasnt_fulfilled_or_errored", should_successfuly_set_state_to_RUNNING_when_the_promise_is_running_and_hasnt_fulfilled_or_errored },
    { "should_successfuly_set_state_to_RUNNING_when_a_deferred_promise_is_reexecuted", should_successfuly_set_state_to_RUNNING_when_a_deferred_promise_is_reexecuted },
    { "should_successfuly_set_state_to_RESULT_when_a_deferred_promise_is_reexecuted_with_fulfill", should_successfuly_set_state_to_RESULT_when_a_deferred_promise_is_reexecuted_with_fulfill },
    { "should_successfuly_set_state_to_ERROR_when_a_deferred_promise_is_reexecuted_with_reject", should_successfuly_set_state_to_ERROR_when_a_deferred_promise_is_reexecuted_with_reject },
    { "should_error_when_a_promise_is_reexecuted_when_it_is_in_state_RUNNING_and_not_in_state_READY_or_DEFER", should_error_when_a_promise_is_reexecuted_when_it_is_in_state_RUNNING_and_not_in_state_READY_or_DEFER },
    { "should_error_when_a_promise_is_reexecuted_when_it_is_in_state_ERROR_and_not_in_state_READY_or_DEFER", should_error_when_a_promise_is_reexecuted_when_it_is_in_state_ERROR_and_not_in_state_READY_or_DEFER },
    { "should_error_when_a_promise_is_reexecuted_when_it_is_in_state_RESULT_and_not_in_state_READY_or_DEFER", should_error_when_a_promise_is_reexecuted_when_it_is_in_state_RESULT_and_not_in_state_READY_or_DEFER },
    { "should_successfuly_set_state_to_RESULT_with_a_passed_in_parameter_when_the_promise_calls_fulfill", should_successfuly_set_state_to_RESULT_with_a_passed_in_parameter_when_the_promise_calls_fulfill },
    { "should_successfuly_pass_args_into_promise", should_successfuly_pass_args_into_promise },
    TEST_END
};
