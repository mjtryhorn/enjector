/*
* async_tests.h
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

#include "async_tests.h"

#include <enjector/core/async.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <string.h>
#include <stdio.h>

static async(async_test_empty) {
}

static void should_successfully_set_the_state_to_READY_when_the_actor_isnt_running() {
    async_task_scheduler_init(false);

    async_task* t = go(async_test_empty, NULL);

    TEST_ASSERT_EQUAL_INT(t->state, TaskStateReady);
    async_task_free(t);

    async_task_scheduler_deinit();

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_result) {
    promise_complete(1234);
}

static async(async_test_call_empty_promise) {
    async_begin {
        // This will call the promise above and wait for the promise to complete
        await_promise(promise_test_result, NULL);
    }
    async_end;
}

static void should_run_the_actor_process_by_awaiting_a_promise() {

    async_task_scheduler_init(false);

    async_task* t = go(async_test_call_empty_promise, NULL);

    // Start the actor using the scheduler
    async_task_scheduler_run(async_task_scheduler_current_get());

    // Check the task is finished
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateFinish);
    TEST_ASSERT_PTR_NULL(t->error);

    // Check the promise is finished too
    TEST_ASSERT_EQUAL_INT(t->promise.state, PromiseStateDefault);

    async_task_free(t);

    async_task_scheduler_deinit();

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_error) {
    // This should cause the actor to stop with an ERROR
    promise_reject("An error");
}

static async(actor_test_error) {
    async_begin {
        // This will call the promise above and wait for the promise to complete
        await_promise(promise_test_error, NULL);
    }
    async_end;
}

static void should_stop_the_actor_process_when_an_exception_is_raised_by_an_awaiting_promise() {

    async_task_scheduler_init(false);

    async_task* t = go(actor_test_error, NULL);

    // Start the actor using the scheduler
    async_task_scheduler_run(async_task_scheduler_current_get());

    // Check the task is failed too
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateError);
    TEST_ASSERT_EQUAL_STRING(t->error, "An error");

    // Check the promise is failed too
    TEST_ASSERT_NOT_EQUAL_INT_FATAL(t->promise.state, PromiseStateDefault);
    TEST_ASSERT_EQUAL_INT(t->promise.state, PromiseStateError);
    TEST_ASSERT_EQUAL_STRING(t->promise.error, "An error");

    async_task_free(t);

    async_task_scheduler_deinit();

    xmemory_report_exit_on_leaks();
}

static int test_did_continue = 0;
static int test_result = 0;
static int test_did_async_catch = 0;
static const char* test_error = "";

static async(actor_test_async_try_async_catch) {
    async_begin {
        async_try
        {
            // This will call the promise above and wait for the promise to complete
            await_promise(promise_test_error, NULL);
        }
        async_catch
        {
            test_error = error;
            test_did_async_catch = 1;
        }

        test_did_continue = 1;
    }
    async_end;
}

static void should_not_stop_the_actor_process_when_a_promise_exception_is_suppressed_by_a_async_try_async_catch_block() {

    async_task_scheduler_init(false);

    test_did_continue = 0;
    test_result = 0;
    test_did_async_catch = 0;
    test_error = "";

    async_task* t = go(actor_test_async_try_async_catch, NULL);

    // Start the actor using the scheduler
    async_task_scheduler_run(async_task_scheduler_current_get());

    // Check the task is failed too
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateFinish);
    TEST_ASSERT_EQUAL_INT(1, test_did_async_catch);
    TEST_ASSERT_EQUAL_STRING(t->error, "An error");
    TEST_ASSERT_EQUAL_INT(1, test_did_continue);

    async_task_free(t);

    async_task_scheduler_deinit();

    xmemory_report_exit_on_leaks();
}

static promise(promise_test_fulfil) {
    promise_complete((void*) 4);
};

static async(actor_test_error_then_continue) {
    async_begin {
        async_try
        {
            // This will call the promise above and wait for the promise to complete
            await_promise(promise_test_error);
        }
        async_catch
        {
            test_error = error;
            test_did_async_catch = 1;
        }

        test_did_continue = 1;

        await_promise(promise_test_fulfil, NULL, long, result) {
            test_result = (int) result;
        }

        test_did_continue = 2;
    }
    async_end;
}

static void should_continue_after_an_async_catch_when_a_promise_exception_occurs() {

    async_task_scheduler_init(false);

    test_did_continue = 0;
    test_result = 0;
    test_did_async_catch = 0;
    test_error = "";

    async_task* t = go(actor_test_error_then_continue, NULL);

    // Start the actor using the scheduler
    async_task_scheduler_run(async_task_scheduler_current_get());

    // Check the task is failed too
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateFinish);
    TEST_ASSERT_EQUAL_INT(1, test_did_async_catch);
    TEST_ASSERT_EQUAL_STRING(t->error, "An error");
    TEST_ASSERT_EQUAL_INT(2, test_did_continue);
    TEST_ASSERT_EQUAL_INT(4, test_result);

    async_task_free(t);

    async_task_scheduler_deinit();

    xmemory_report_exit_on_leaks();
}

static promise(promise_do_nothing) {
    promise_defer();
};

static async(async_do_echo) {
    async_begin {
        await_promise(promise_do_nothing, NULL);
    }
    async_end;
}

static void should_successfully_run_scheduler_for_specified_duration() {
    async_task_scheduler_init(false);

    async_task* t = go(async_do_echo, NULL);

    clock_stopwatch stopwatch;
    clock_stopwatch_start(&stopwatch);

    // Start the actor use the scheduler for a period
    async_task_scheduler_runfor(async_task_scheduler_current_get(), 100);

    // Check we did have the task still running
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateRunning);

    TEST_ASSERT_TRUE(clock_stopwatch_elapsed(&stopwatch) >= 100);
    TEST_ASSERT_TRUE(clock_stopwatch_elapsed(&stopwatch) < 200);       // Within a tolerance

    async_task_free(t);
    async_task_scheduler_deinit();
    xmemory_report_exit_on_leaks();
}

/*static void should_successfully_set_the_state_to_RUNNING_when_the_actor_is_running() {


	actor1 a;

	// Start the actor using the scheduler
	Scheduler s;
	s.go(a);

	// Run a couple of times for the actor to pass the promise for schduled execution and the promise to execute.
	s.poll();
	s.poll();

	expect(a.state).toEqual(TaskStateRunning);

	// Check the promise is still running too
	expect(a.promise).Not().toBeNull();
	expect(a.promise.state).toEqual(PromiseStateRunning);
 });
 */


static promise(promise_return_result) {
    promise_complete("Hello World");
};

static async(async_test_check_result_from_promise) {
    async_begin {
        await_promise_result(promise_return_result, NULL, char*, result);
        TEST_ASSERT_EQUAL_STRING("Hello World", result);
    }
    async_end;
}

static void should_successfully_return_result_from_await_promise() {
    async_task_scheduler_init(false);

    async_task* t = go(async_test_check_result_from_promise, NULL);

    // Start the actor use the scheduler for a period
    async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());

    // Check we did have the task still running
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateFinish);

    async_task_free(t);
    async_task_scheduler_deinit();
    xmemory_report_exit_on_leaks();
}

static async(async_test_chain_2_noargs) {
    async_begin {
        await_promise_result(promise_return_result, NULL, char*, result);
        TEST_ASSERT_EQUAL_STRING("Hello World", result);
        static char buffer[100];
        sprintf_s(buffer, sizeof(buffer), "Chained result: %s", result);
        async_return(buffer);
    }
    async_end;
}

static async(async_test_chain_1_noargs) {
    async_begin {
        await(async_test_chain_2_noargs, NULL, char*, result);
        TEST_ASSERT_EQUAL_STRING("Chained result: Hello World", result);
        async_return("OK");
    }
    async_end;
}

static void should_successfully_return_result_from_await_awaited_task_noargs() {
    async_task_scheduler_init(true);

    go(async_test_chain_1_noargs, NULL);

    // Start the actor use the scheduler for a period
    async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());

    async_task_scheduler_deinit();
    xmemory_report_exit_on_leaks();
}

static async(async_test_params, char*, name) {
    async_begin {
        async_return(text_format("Hello %s", name));
    }
    async_end;
}

static void should_successfully_pass_params_to_async() {
    async_task_scheduler_init(false);

    async_task* t = go(async_test_params, "Fred");

    // Start the actor use the scheduler for a period
    async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());

    // Check we did have the task still running
    TEST_ASSERT_EQUAL_INT(t->state, TaskStateFinish);

    TEST_ASSERT_EQUAL_STRING("Hello Fred", t->result);
    text_free(t->result);

    async_task_free(t);
    async_task_scheduler_deinit();
    xmemory_report_exit_on_leaks();
}

static async(async_test_chain_2_args, char*, name) {
    async_begin {
        await_promise_result(promise_return_result, NULL, char*, result);
        TEST_ASSERT_EQUAL_STRING("Hello World", result);
        static char buffer[100];
        sprintf_s(buffer, sizeof(buffer), "Hello %s", name);
        async_return(buffer);
    }
    async_end;
}

static async(async_test_chain_3_args, char*, name) {
    async_begin {
        await_promise_result(promise_return_result, NULL, char*, result);
        TEST_ASSERT_EQUAL_STRING("Hello World", result);
        static char buffer[100];
        sprintf_s(buffer, sizeof(buffer), "Bye %s", name);
        async_return(buffer);
    }
    async_end;
}

static async(async_test_chain_1_args, char*, name) {
    async_begin {
        await(async_test_chain_2_args, name, char*, result1);
        static char expected1[100];
        sprintf_s(expected1, sizeof(expected1), "Hello %s", name);
        TEST_ASSERT_EQUAL_STRING(expected1, result1);

        await(async_test_chain_2_args, name, char*, result2);
        static char expected2[100];
        sprintf_s(expected2, sizeof(expected2), "Hello %s", name);
        TEST_ASSERT_EQUAL_STRING(expected2, result2);

        await(async_test_chain_3_args, name, char*, result3);
        static char expected3[100];
        sprintf_s(expected3, sizeof(expected3), "Bye %s", name);
        TEST_ASSERT_EQUAL_STRING(expected3, result3);

        async_return("OK");
    }
    async_end;
}

static void should_successfully_return_result_from_await_awaited_task_args() {
    async_task_scheduler_init(true);

    go(async_test_chain_1_args, "Bob");

    // Start the actor use the scheduler for a period
    async_task_scheduler_run_until_no_tasks(async_task_scheduler_current_get());

    async_task_scheduler_deinit();
    xmemory_report_exit_on_leaks();
}

test async_tests[] = {
    { "should_successfully_set_the_state_to_READY_when_the_actor_isnt_running", should_successfully_set_the_state_to_READY_when_the_actor_isnt_running },
    { "should_run_the_actor_process_by_awaiting_a_promise", should_run_the_actor_process_by_awaiting_a_promise },
    { "should_stop_the_actor_process_when_an_exception_is_raised_by_an_awaiting_promise", should_stop_the_actor_process_when_an_exception_is_raised_by_an_awaiting_promise },
    { "should_not_stop_the_actor_process_when_a_promise_exception_is_suppressed_by_a_async_try_async_catch_block", should_not_stop_the_actor_process_when_a_promise_exception_is_suppressed_by_a_async_try_async_catch_block },
    { "should_continue_after_an_async_catch_when_a_promise_exception_occurs", should_continue_after_an_async_catch_when_a_promise_exception_occurs },
    { "should_successfully_run_scheduler_for_specified_duration", should_successfully_run_scheduler_for_specified_duration },
    { "should_successfully_return_result_from_await_promise", should_successfully_return_result_from_await_promise},
    { "should_successfully_return_result_from_await_awaited_task_noargs", should_successfully_return_result_from_await_awaited_task_noargs },
    { "should_successfully_pass_params_to_async", should_successfully_pass_params_to_async },
    { "should_successfully_return_result_from_await_awaited_task_args", should_successfully_return_result_from_await_awaited_task_args },
    TEST_END
};
