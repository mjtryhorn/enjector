/*
* async.c
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

#include <enjector/core/features.h>
#include <enjector/core/async.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _MSC_VER
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// So that we can shared this between modules
// https://msdn.microsoft.com/en-us/library/h90dkhs0(VS.80).aspx
#pragma data_seg (".myseg")

#endif

static long _global_task_counter = 0;
static async_task_scheduler* _current_scheduler = NULL;

#pragma data_seg()

async_task* async_task_create(const char* name, async_task_cb_run task_run, void* ctx, async_future* future, void* context) {
    assert(task_run);

    async_task* task = xmemory_new(async_task);
    task->state = TaskStateReady;
    task->_line = -1;
    task->promise.state = PromiseStateDefault;
    task->error = NULL;
    task->id = _global_task_counter++;
    task->exit_on_error = true;
    task->run = task_run;
    task->ctx = ctx;
    task->name = name ? text_clone(name) : NULL;
    task->future = future;
    task->context = context;

#ifdef ASYNC_CAPTURE_TASK_TIMINGS
    clock_stopwatch_start(&task->stopwatch);
#endif

    return task;
}

void async_task_free(async_task* task) {
    assert(task);

    //    task->state = TaskStateFinish;

    if(task->name) xmemory_free(task->name);

    xmemory_free(task);
}

void async_task_scheduler_add(async_task_scheduler* scheduler, async_task* task) {
    assert(task);

    list_add(scheduler->tasks, task);
}

task_state async_task_scheduler_exec(async_task* task) {
    assert(task);

    //printf("Running task %s\n", task->name ? task->name : "(unknown)");

    //promise* oldPromise = NULL;
    if(task->state == TaskStateSuspend || task->state == TaskStateFinish || task->state == TaskStateFinal) {
        // Don't do anything
        return task->state;
    }

    // For those actors that has not finished or errored ...
    if(task->state != TaskStateStop && task->state != TaskStateError) {

        // TODO: couroutine state
        if(task->promise.state == PromiseStateDefault) {

            async_task_scheduler_performance_task_start(async_task_scheduler_current_get(), task);

            // We havent executed the coroutine, so execute with a placeholder of a promise.
            task->run(task, task->ctx, task->future, task->context);    // NOTE: if you get BAD ACCESS it due to the async routine being out of scope

            task->state = TaskStateRunning;

            // At this point the actor may have finished and hence isFinished will
            // be true. However, if a promise came out, it will be executed and passed
            // back to this coroutine.

            // TODO: maybe put the isFinished here based on the return.

            // Existing promise available, so process that
        } else {

            // Is the coroutine has an attached promise, let's process the promise until it
            // completes its execution lifecycle.

            //Call the actor cortinue again if the promise has a result
            switch(task->promise.state) {
            case PromiseStateReady:

                // TODO: pass in scheduler
                async_task_scheduler_performance_promise_start(async_task_scheduler_current_get(), &task->promise);

                // The promise is in a state that it is ready to execute.
                promise_run(&task->promise);
                //t->promise.run(t->promise, t->promise.ctx);
                break;

            case PromiseStateResult:

                // The promise called fulfill() and we have a result. Call the coroutine again so it
                // can resume execution.

                // TODO: pass in scheduler
                async_task_scheduler_performance_promise_end(async_task_scheduler_current_get(), &task->promise);

                // Keep hold of old promise since it may be replaced
                //					oldPromise = task->promise;

                // Call the coroutine with the promise result
                task->run(task, task->ctx, task->future, task->context);
                //task->promise.state = PromiseStateDefault;	// Clear the promise

                // Set the old promise into a FINISH state, so it can be cleaned up.
                //	oldpromise.state = PromiseStateFinish;
                //	xmemory_free(oldPromise);

                break;

            case PromiseStateError:

                // Keep hold of old promise since it may be replaced
                //	oldPromise = task->promise;

                // The promise called reject() or raised an error. Has the coroutine set up
                // an async_try ... async_catch? If not then the coroutine will terminate.
                if(!task->exit_on_error) {

                    // We're not exiting the coroutine on error, the coroutine
                    // will manage the error in it's async_catch block.

                    // Call the coroutine with the promise error
                    //task->promise.state = PromiseStateDefault;
                    task->run(task, task->ctx, task->future, task->context);

                    task->state = TaskStateRunning;
                }

                // Set the promise to FINISH state, so it can be cleaned up.
                //oldpromise.state = PromiseStateFinish; // TODO: move to the promise class
                //	xmemory_free(oldPromise);

                break;

            case PromiseStateDefer:
                // The promise was deferred using defer(), which means we will re-execute the promise
                //					t->promise.run(t->promise, t->promise.ctx);
                promise_run(&task->promise);

                break;

            //case PromiseStateFinish:
            //	// Promise finished, so clean up.
            //	a->promise = NULL;

            //	// TODO: move clean up code triggers here
            //	break;
            case PromiseStateRunning:
                // Don't do anything, we're waiting for fulfil or reject to happen
                break;

            default:
                fprintf(stderr, "WARNING: dont know how to handle state %d\n", task->promise.state);
                break;
            }
        }
    }

    // Have we finished - ie. no more promises to execute for the actor?
    if(task->promise.state == PromiseStateDefault) {
        task->state = TaskStateFinal;
    }

    // Check for promise errors
    else if(task->promise.state == PromiseStateError) {
        task->error = task->promise.error;

        if(task->exit_on_error) {
            task->state = TaskStateError;
        } else {
            task->state = TaskStateException;
        }
    }

    // Process errors: the default behaviour is to shutdown the actor process,
    // but if the coroutine has a async_try ... async_catch block, then pass the error back through
    /*     if (has_error && a->exit_on_error) {
     fprintf(stderr, "error: %s\n", error_message.c_str());
     }
     */
    return task->state;
}

async_task* async_task_scheduler_go(async_task_scheduler* scheduler, const char* name, async_task_cb_run task_run, void* ctx, async_future* future, void* context) {
    assert(scheduler);
    assert(task_run);

    async_task* task = async_task_create(name, task_run, ctx, future, context);

    //// Find an empty slot
    //bool saved = false;

    ////	list_item** items = list_enumerable(scheduler->tasks);
    //int items_len = list_count(scheduler->tasks);

    //for (int i = 0; i < items_len; i++) {
    //	list_item* item = list_get_item(scheduler->tasks, i);

    //	if (item->value == NULL) {
    //		item->value = task;
    //		saved = true;
    //		break;
    //	}
    //}

//	if (saved == false) {
    async_task_scheduler_add(scheduler, task);
    //	saved = true;
//	}

    //printf("Tasks %d\n", scheduler->tasks->length);

    return task;
}

void async_task_scheduler_remove_all(async_task_scheduler* scheduler, void* ctx) {
    assert(scheduler);
    assert(ctx);

    size_t items_len = list_count(scheduler->tasks);

    size_t i = 0;

    while(i < items_len) {
        list_item* item = list_get_item(scheduler->tasks, i);

        bool is_removed = false;

        if(item->value != NULL) {
            async_task* task = (async_task*) item->value;

            if(task->ctx == ctx) {
                async_task_free(task);
                list_remove_item_at(scheduler->tasks, i);

                // We have removed one
                is_removed = true;
                items_len--;
            }
        }

        // Move to the next index, otherwise stay at the same index position and move along
        if(!is_removed) {
            i++;
        }
    }
}

void async_task_scheduler_remove_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx) {
    assert(scheduler);
    assert(ctx);
    assert(task_name);

    size_t items_len = list_count(scheduler->tasks);

    size_t i = 0;

    while(i < items_len) {
        list_item* item = list_get_item(scheduler->tasks, i);

        bool is_removed = false;

        if(item->value != NULL) {
            async_task* task = (async_task*) item->value;

            if(task->ctx == ctx && task->name && text_equals(task->name, task_name)) {
                async_task_free(task);
                list_remove_item_at(scheduler->tasks, i);

                // We have removed one
                is_removed = true;
                items_len--;
            }
        }

        // Move to the next index, otherwise stay at the same index position and move along
        if(!is_removed) {
            i++;
        }
    }
}

void async_task_scheduler_stop_all(async_task_scheduler* scheduler, void* ctx) {
    assert(scheduler);
    assert(ctx);

    list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        if(task->ctx == ctx) {
            task->state = TaskStateStop;
        }
    }
    list_foreach_of_end
}

void async_task_scheduler_stop_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx) {
    assert(scheduler);
    assert(ctx);
    assert(task_name);

    list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        if(task->ctx == ctx && task->name && text_equals(task->name, task_name)) {
            task->state = TaskStateStop;
        }
    }
    list_foreach_of_end
}

void async_task_scheduler_suspend_all(async_task_scheduler* scheduler, void* ctx) {
    assert(scheduler);
    assert(ctx);

    size_t items_len = list_count(scheduler->tasks);

    size_t i = 0;

    list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        if(task->ctx == ctx && task->state == TaskStateRunning) {
            task->state = TaskStateSuspend;
        }
    }
    list_foreach_of_end
}

void async_task_scheduler_suspend_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx) {
    assert(scheduler);
    assert(ctx);
    assert(task_name);

    list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        if(task->ctx == ctx && task->name && text_equals(task->name, task_name) && task->state == TaskStateRunning) {
            task->state = TaskStateRunning;
        }
    }
    list_foreach_of_end
}

void async_task_scheduler_resume_all(async_task_scheduler* scheduler, void* ctx) {
    assert(scheduler);
    assert(ctx);

    list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        if(task->ctx == ctx && task->state == TaskStateSuspend) {
            task->state = TaskStateRunning;
        }
    }
    list_foreach_of_end
}

void async_task_scheduler_resume_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx) {
    assert(scheduler);
    assert(ctx);
    assert(task_name);

    list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        if(task->ctx == ctx && task->name && text_equals(task->name, task_name) && task->state == TaskStateSuspend) {
            task->state = TaskStateRunning;
        }
    }
    list_foreach_of_end
}

bool async_task_scheduler_poll(async_task_scheduler* scheduler) {

    assert(scheduler);

    bool stillRunning = false;


    // Holds the positions of the tasks to remove when they're complete
    //	list* tasks_still_running = list_create();

    //	list_item** items = list_enumerable(scheduler->tasks);
    size_t items_len = list_count(scheduler->tasks);

    for(size_t i = 0; i < items_len; i++) {
        //	for (int i = 0; i < list_count(scheduler->tasks); i++)
        //	{
        list_item* item = list_get_item(scheduler->tasks, i);

        //		if (item == NULL) continue;

        // Skip freed slots
        async_task* task = (async_task*) item->value;

        if(task) {
            const task_state state = async_task_scheduler_exec(task);

            // Remove tasks which have finished or errored
            /*	if (state == TaskStateFinal || state == TaskStateError)
             {
             // If the scheduler is set up with auto destroy enabled, then delete finished tasks
             if (scheduler->auto_destroy)
             {
             async_task_free(task);

             }

             // Mark the task as completed, it will be removed later - just incase
             item = list_get_item(scheduler->tasks, i);
             item->value = NULL;
             }
             */
            // See if the task has been finished
            if(state == TaskStateReady || state == TaskStateRunning || state == TaskStateSuspend || state == TaskStateException) {

                stillRunning = true;

                // Add the running tasks onto another list
                // TODO: ideally use linked list
                //		list_add(tasks_still_running, task);
            } else if(state == TaskStateFinal) {
                stillRunning = true;

                // This allows any awaits to get data
                task->state = TaskStateFinish;
            } else {
#ifdef ASYNC_CAPTURE_TASK_TIMINGS
                // TODO: calculate the hit count and elapsed time
                async_task_scheduler_performance_task_end(async_task_scheduler_current_get(), task);
#endif

                // Remove tasks which have finished or errored
                // Mark the task as completed, it will be removed later - just incase
                //item = list_get_item(scheduler->tasks, i);
                ////				xmemory_free(item->value);
                //item->value = NULL;
                //printf("-%d\n", i);

                list_remove_item_at(scheduler->tasks, i);

                // Make sure we revisit slot
                i--;
                items_len--;

                // If the scheduler is set up with auto destroy enabled, then delete finished tasks
                if(scheduler->auto_destroy) {
                    async_task_free(task);
                }
            }
        }
    }

    //	list_free(scheduler->tasks);
    //scheduler->tasks = tasks_still_running;

    return stillRunning;
}

void async_task_scheduler_run(async_task_scheduler* scheduler) {
    assert(scheduler);

    while(async_task_scheduler_poll(scheduler)) {
        //system_sleep(TIMER_FREQ_ASYNC_SCHEDULER); // Should be min scheduler - TODO: adapative based on
        //printf(".");
        // TODO: Build adaptive
    }
}

void async_task_scheduler_runfor(async_task_scheduler* scheduler, long double duration) {
    assert(scheduler);

    clock_stopwatch stopwatch;
    clock_stopwatch_start(&stopwatch);

    //int min = 1000;
    //int n = 1000;

    while(async_task_scheduler_poll(scheduler) && clock_stopwatch_elapsed(&stopwatch) < duration) {

        //   system_sleep(TIMER_FREQ_ASYNC_SCHEDULER); // Should be min scheduler - TODO: adapative based on

        // TODO: Build adaptive

        //if (n-- == 0)
        //{
        //	list_foreach_of_begin(scheduler->tasks, async_task*, task) {
        //		if (task->promise.ctx) {
        //			async_task_free(task);
        //		}
        //	}
        //	list_foreach_of_end
        //}

        //	sleepms(5);
        //        printf(".");
    }
}

void async_task_scheduler_run_until_no_tasks(async_task_scheduler* scheduler) {
    assert(scheduler);

    size_t tasks_count;

    while(tasks_count = list_count(scheduler->tasks), tasks_count > 0 && async_task_scheduler_poll(scheduler)) {
        //		system_sleep(TIMER_FREQ_ASYNC_SCHEDULER); // Should be min scheduler - TODO: adapative based on
        //printf(".");
        // TODO: Build adaptive
    }
}

void async_task_scheduler_run_async_handler(int signum) {
    static int count = 0;
    printf("timer expired %d times\n", ++count);
}


void async_task_scheduler_runasync(async_task_scheduler* scheduler) {
    assert(scheduler);
    //
    //struct sigaction sa;
    //struct itimerval timer;
    //
    ///* Install timer_handler as the signal handler for SIGVTALRM. */
    //memset (&sa, 0, sizeof (sa));
    //sa.sa_handler = &async_task_scheduler_run_async_handler;
    //sigaction (SIGVTALRM, &sa, NULL);
    //
    ///* Configure the timer to expire after 250 msec... */
    //timer.it_value.tv_sec = 0;
    //timer.it_value.tv_usec = 250000;
    ///* ... and every 250 msec after that. */
    //timer.it_interval.tv_sec = 0;
    //timer.it_interval.tv_usec = 250000;
    ///* Start a virtual timer. It counts down whenever this process is
    // executing. */
    //setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

async_task_scheduler* async_task_scheduler_create(bool auto_destroy) {
    async_task_scheduler* scheduler = xmemory_new(async_task_scheduler);
    scheduler->tasks = list_create();
    scheduler->auto_destroy = auto_destroy;	// Auto destroy on last use

    // TODO
//	scheduler->performance_promise = map_create();
    //scheduler->performance_task = map_create();
    return scheduler;
}

void async_task_scheduler_free(async_task_scheduler* scheduler) {
    assert(scheduler);
    assert(scheduler->tasks);

    if(scheduler->auto_destroy) {
        list_foreach_of_begin(scheduler->tasks, async_task*, task) {
            if(task) {
                async_task_free(task);
            }
        }
        list_foreach_of_end
    }

    list_free(scheduler->tasks);
    xmemory_free(scheduler);
}

async_task_scheduler* async_task_scheduler_current_get() {
    // Set default scheduler if not set already
    if(!_current_scheduler) {
        async_task_scheduler_init(true);
    }

    //    assert(_current_scheduler); // You must call scheduler_init() first
    return _current_scheduler;
}

void async_task_scheduler_init(bool auto_destroy) {
    if(_current_scheduler == NULL) {
        _current_scheduler = async_task_scheduler_create(auto_destroy);
    }
}

void async_task_scheduler_deinit() {
    if(_current_scheduler) {
        async_task_scheduler_free(_current_scheduler);
        _current_scheduler = NULL;
    }
}

void async_task_scheduler_stats(string_buffer* stream, async_task_scheduler* scheduler) {
    assert(scheduler);

    string_buffer_append_format(stream, "=======================================================================\n");
    string_buffer_append_format(stream, ">> Scheduled Tasks\n");
    string_buffer_append_format(stream, "=======================================================================\n");
    string_buffer_append_format(stream, "Context   Task                                                  State\n");
    string_buffer_append_format(stream, "-----------------------------------------------------------------------\n");
    list* tasks = scheduler->tasks;
    list_foreach_of_begin(tasks, async_task*, task) {
        string_buffer_append_format(stream, "%-10X  %-50s  %s\n", task->ctx, task->name, async_task_state_state_to_string(task->state));
    } list_foreach_of_end

    printf("\n\n");
}

const char*	async_task_state_state_to_string(task_state state) {
    switch(state) {
    case TaskStateNotSet:
        return "Not Set";

    case TaskStateReady:
        return "Ready";

    case TaskStateRunning:
        return "Running";

    case TaskStateError:
        return "Error";

    case TaskStateException:
        return "Exception";

    case TaskStateFinal:
        return "Final";

    case TaskStateStop:
        return "Stop";

    case TaskStateFinish:
        return "Finish";

    default:
        return "(unknown)";
    }
}

void async_task_scheduler_stats_show(async_task_scheduler* scheduler) {
    string_buffer* stream = string_buffer_create_empty();
    async_task_scheduler_stats(stream, scheduler);
    puts(string_buffer_get(stream));
    string_buffer_free(stream);
}

void async_task_scheduler_performance_promise_start(async_task_scheduler* s, promise* p) {
    //printf("Promise start %s\n", p->name);
#ifdef ASYNC_CAPTURE_TASK_TIMINGS
    clock_stopwatch_start(&p->stopwatch);
#endif
}

void async_task_scheduler_performance_promise_end(async_task_scheduler* s, promise* p) {
#ifdef ASYNC_CAPTURE_TASK_TIMINGS
    clock_stopwatch_end(&p->stopwatch);
    //printf("Promise end %s %lg\n", p->name, clock_stopwatch_elapsed(&p->stopwatch));
#endif
}

void async_task_scheduler_performance_task_start(async_task_scheduler* s, async_task* t) {
    //	printf("Task start %s\n", t->name);
#ifdef ASYNC_CAPTURE_TASK_TIMINGS
    clock_stopwatch_start(&t->stopwatch);
#endif
}

void async_task_scheduler_performance_task_end(async_task_scheduler* s, async_task* t) {
#ifdef ASYNC_CAPTURE_TASK_TIMINGS
    clock_stopwatch_end(&t->stopwatch);
#endif
    //printf("Task end %s %lg\n", t->name, clock_stopwatch_elapsed(&t->stopwatch));
}

promise(async_task_to_complete, async_task*, t) {
    switch(t->state) {
    case TaskStateFinish:
        promise_complete(t->result);
        break;

    case TaskStateReady:
        promise_defer();
        break;

    case TaskStateRunning:
        promise_defer();
        break;

    case TaskStateError:
        promise_reject(t->error);
        break;

    case TaskStateException:
        promise_reject(t->error);
        break;

    case TaskStateStop:
        promise_complete(NULL);
        break;

    case TaskStateSuspend:
        promise_complete(NULL);
        break;

    case TaskStateNotSet:
        promise_reject("bad state");
        break;

    case TaskStateFinal:
        promise_complete(t->result);
        break;
    }
}

async_future* async_future_create() {
    async_future* future = xmemory_new(async_future);
    return future;
}

void async_future_wait(async_future* future) {
    assert(future);

    // TODO: poll too?
    while(future->state == AsyncFutureUnknown && async_task_scheduler_poll(async_task_scheduler_current_get())) {
//		system_sleep(0);
    }
}

void async_future_dispose(async_future* future) {
    assert(future);

    if(future->error) xmemory_free(future->error);

    xmemory_free(future);
}

async_future_state async_future_state_get(async_future* future) {
    assert(future);
    return future->state;
}

void async_future_value_set(async_future* future, void* value) {
    assert(future);
    future->state = AsyncFutureOK;
    future->value = value;
}

void* async_future_value_get(async_future* future) {
    assert(future);
    return future->value;
}

void async_future_error_set(async_future* future, const char* error) {
    assert(future);
    assert(error);

    future->state = AsyncFutureError;
    future->error = text_clone(error);
}

char* async_future_error_get(async_future* future) {
    assert(future);
    return future->error;
}
