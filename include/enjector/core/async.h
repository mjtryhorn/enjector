/*
* async.h
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

#include <enjector/core/promise.h>
#include <enjector/core/list.h>
#include <enjector/core/map.h>
#include <enjector/core/string_buffer.h>

/******************************************************************************
 * Future
 */
typedef enum {
    AsyncFutureUnknown,
    AsyncFutureOK,
    AsyncFutureError
} async_future_state;

typedef struct {
    void*				value;

    async_future_state	state;
    char*				error;
} async_future;

async_future*		async_future_create();
void				async_future_dispose(async_future* future);
void				async_future_wait(async_future* future);

async_future_state	async_future_state_get(async_future* future);

void				async_future_value_set(async_future* future, void* value);
void*				async_future_value_get(async_future* future);

void				async_future_error_set(async_future* future, const char* error);
char*				async_future_error_get(async_future* future);

/******************************************************************************
 * Tasks
 */

typedef enum task_state_t {
    TaskStateNotSet = 0,
    TaskStateReady = 1,         // Hasn't been run
    TaskStateRunning = 2,       // Is Running
    TaskStateError = 3,         // An error ocurred
    TaskStateException = 4,     // An error has occured by a promise which is going to be handled by an async_catch
    TaskStateFinal = 5,		    // Is ready for disposal
    TaskStateStop = 6,		    // Has been stopped
    TaskStateSuspend = 7,	    // Has been suspended,
    TaskStateFinish = 8         // Has completed and not used
} task_state;

typedef struct async_task_t async_task;
typedef void (*async_task_cb_run)(async_task* task, void* _ctx, async_future* future, void* context);

typedef struct async_task_t {
    int                 id;
    char*               name;

    promise             promise;    // The current promise context set by a recent await in the actor
    const char*         error;      // Holds the last error message
    void*               result;		// Holds any resultant value
    async_future*       future;	    // Any passed with future associated with task

    task_state          state;
    int                 _line;

    bool                exit_on_error;	 // TODO; should be exit procvess/corout on error

    // A pointer to the callback routine that will run
    async_task_cb_run   run;

    void*               ctx;	// Context: holds args passed into the task

    void*               context;

    clock_stopwatch     stopwatch;
} async_task;


async_task* async_task_create(const char* name, async_task_cb_run task_run, void* ctx, async_future* future, void* context);
void		async_task_free(async_task* task);

#define await_emit(T) \
    if(task->_line < 0) {\
    task->_line = 0;}\
    switch (task->_line) {\
    case 0:

#define async_end default: break; }\
    _async_end:\
    task->_line = 0;\
    task->state = TaskStateFinish;\
    if (_p != NULL) { _p = NULL; } \
    return;

#define async_with_arg(fn_name, ct, c) void fn_name(async_task* task, ct c, async_future* future, void* context)
#define async_with_noarg(fn_name) void fn_name(async_task* task, void* _ctx, async_future* future, void* context)

#ifdef _WIN32

#pragma warning(disable:4002)
#pragma warning(disable:4003)
#define ASYNC_SELMACRO_IMPL(_1, _2, _3, N,...) N
#define ASYNC_SELMACRO_IMPL_(tuple) ASYNC_SELMACRO_IMPL tuple
#define async(...) ASYNC_SELMACRO_IMPL_((__VA_ARGS__, async_with_arg(__VA_ARGS__), NULL, async_with_noarg(__VA_ARGS__)))

#define async_arg(name) _ctx && name ? map_get_value((map*) _ctx, name) : NULL

#else

#define ASYNC_GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define ASYNC_MACRO_CHOOSER(...) ASYNC_GET_4TH_ARG(__VA_ARGS__, async_with_arg, , async_with_noarg)
#define async(...) ASYNC_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


#endif

#define async_yield(V) \
    task->_line = __LINE__;\
    return;\
    case __LINE__:;

#define async_begin promise* _p = NULL; int _pstate = PromiseStateDefault; await_emit(promise*)

#define await_promise_result(f, c, vt, v) \
    _p = promise_init(&task->promise, f, c); \
    async_yield(_p); \
    vt v = 0;\
    _p = (promise*) &task->promise; if (_p->state == PromiseStateResult) { v = (vt) _p->result; } _p->state = PromiseStateDefault;

#define await_promise_noresult2(f, c) \
    _p = promise_init(&task->promise, f, c); \
    async_yield(_p) \
    _p = (promise*) &task->promise; _p->state = PromiseStateDefault;

#define await_promise_noresult1(f) await_promise_noresult2(f, NULL)

promise(async_task_to_complete, async_task*, t);

#define await_task(t, vt, v) await_promise(async_task_to_complete, t, vt, v)

#define await_result(f, c, vt, v) \
    async_task* _t ## f ## v = go(f, c); \
    await_task(_t ## f ## v, vt, v);\

#define await_noresult(f, c) \
    async_task* _t ## f = go(f, c); \
    await_promise(async_task_to_complete, _t ## f);

#ifdef _WIN32

#pragma warning(disable:4002)
#pragma warning(disable:4003)
#define AWAIT_SELMACRO_IMPL(_1, _2, _3, _4, N,...) N
#define AWAIT_SELMACRO_IMPL_(tuple) AWAIT_SELMACRO_IMPL tuple

#define await(...) AWAIT_SELMACRO_IMPL_((__VA_ARGS__, await_result(__VA_ARGS__), __ERROR__missing_type__, await_noresult(__VA_ARGS__), __ERROR__missing_type__))
#define await_promise(...) AWAIT_SELMACRO_IMPL_((__VA_ARGS__, await_promise_result(__VA_ARGS__), __ERROR__missing_type__, await_promise_noresult2(__VA_ARGS__), await_promise_noresult1(__VA_ARGS__)))

#else

#define AWAIT_MACRO_CHOOSER_GET_4TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define AWAIT_MACRO_CHOOSER(...) AWAIT_MACRO_CHOOSER_GET_4TH_ARG(__VA_ARGS__, await_result, __ERROR__missing_type__, await_noresult, __ERROR__missing_type__)
#define await(...) AWAIT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


#define AWAIT_PROMISE_MACRO_CHOOSER_GET_4TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define AWAIT_PROMISE_MACRO_CHOOSER(...) AWAIT_PROMISE_MACRO_CHOOSER_GET_4TH_ARG(__VA_ARGS__, await_promise_result, __ERROR__missing_type__, await_promise_noresult2, await_promise_noresult1)
#define await_promise(...) AWAIT_PROMISE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#endif

#define async_return(value) if (future) async_future_value_set(future, value); task->result = value; { goto _async_end; }

// TODO: when async_catch has been run clear the exit_on_error flag
// TODO: look at nested async_try ... async_catch statements
#define async_try task->exit_on_error = false;
#define async_catch const char* error = task->error; task->exit_on_error = true; if (task->state == TaskStateException)
//if (ctx_name) { printf("No context: %s @ %d\n", #ctx_name, __LINE__);  }

///////////////////////////////////////////////////////////////////////////////
// Scheduler
///////////////////////////////////////////////////////////////////////////////

typedef struct async_task_scheduler {
    list*	tasks;
    bool auto_destroy;
    map*	performance_promise;
    map*	performance_task;
} async_task_scheduler;

void		async_task_scheduler_init(bool auto_destroy);
void		async_task_scheduler_deinit();
async_task_scheduler*	async_task_scheduler_create(bool auto_destroy);
void		async_task_scheduler_free(async_task_scheduler* scheduler);
void		async_task_scheduler_run(async_task_scheduler* scheduler);
void		async_task_scheduler_runfor(async_task_scheduler* scheduler, long double duration);
void		async_task_scheduler_run_async(async_task_scheduler* scheduler);
void		async_task_scheduler_run_until_no_tasks(async_task_scheduler* scheduler);
async_task*	async_task_scheduler_go(async_task_scheduler* scheduler, const char* name, async_task_cb_run task_run, void* ctx, async_future* future, void* context);
bool 	async_task_scheduler_poll(async_task_scheduler* scheduler);
task_state	async_task_scheduler_exec(async_task* task);
void		async_task_scheduler_add(async_task_scheduler* scheduler, async_task* task);
void        async_task_scheduler_remove_all(async_task_scheduler* scheduler, void* ctx);
void		async_task_scheduler_remove_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx);
void        async_task_scheduler_stop_all(async_task_scheduler* scheduler, void* ctx);
void		async_task_scheduler_stop_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx);
void        async_task_scheduler_suspend_all(async_task_scheduler* scheduler, void* ctx);
void		async_task_scheduler_suspend_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx);
void        async_task_scheduler_resume_all(async_task_scheduler* scheduler, void* ctx);
void		async_task_scheduler_resume_by_task(async_task_scheduler* scheduler, const char* task_name, void* ctx);
void		async_task_scheduler_stats(string_buffer* stream, async_task_scheduler* scheduler);
void		async_task_scheduler_stats_show(async_task_scheduler* scheduler);

void					async_task_scheduler_current_set(async_task_scheduler* s);
async_task_scheduler*	async_task_scheduler_current_get();

const char*	async_task_state_state_to_string(task_state state);

// TODO: optional ctx
#define		go_with_future(task, ctx, future) async_task_scheduler_go(async_task_scheduler_current_get(), #task, (async_task_cb_run) task, ctx, future, NULL)
#define		go_with_nofuture(task, ctx) async_task_scheduler_go(async_task_scheduler_current_get(), #task, (async_task_cb_run) task, ctx, NULL, NULL)
#define		go_no_context(task) async_task_scheduler_go(async_task_scheduler_current_get(), #task, (async_task_cb_run) task, NULL, NULL, NULL)

#ifdef _WIN32

#pragma warning(disable:4002)
#pragma warning(disable:4003)
#define     GO_SELMACRO_IMPL(_1, _2, _3, _4, N,...) N
#define     GO_SELMACRO_IMPL_(tuple) GO_SELMACRO_IMPL tuple
#define		go(...) GO_SELMACRO_IMPL_((__VA_ARGS__, __ERROR__missing_type__, go_with_future(__VA_ARGS__), go_with_nofuture(__VA_ARGS__), go_no_context(__VA_ARGS__)))

#else

#define     GO_GET_4TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define     GO_MACRO_CHOOSER(...) GO_GET_4TH_ARG(__VA_ARGS__, __ERROR__missing_type__, go_with_future, go_with_nofuture, go_no_context)
#define		go(...) GO_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#endif

#define		stop(task, ctx) if (async_task_scheduler_current_get()) async_task_scheduler_stop_by_task(async_task_scheduler_current_get(), #task, ctx)
#define		stop_all(ctx) if (async_task_scheduler_current_get()) async_task_scheduler_stop_all(async_task_scheduler_current_get(), ctx)
#define		suspend(task, ctx) if (async_task_scheduler_current_get()) async_task_scheduler_suspend_by_task(async_task_scheduler_current_get(), #task, ctx)
#define		suspend_all(ctx) if (async_task_scheduler_current_get()) async_task_scheduler_suspend_all(async_task_scheduler_current_get(), ctx)
#define		resume(task, ctx) if (async_task_scheduler_current_get()) async_task_scheduler_resume_by_task(async_task_scheduler_current_get(), #task, ctx)
#define		resume_all(ctx) if (async_task_scheduler_current_get()) async_task_scheduler_resume_all(async_task_scheduler_current_get(), ctx)

void		async_task_scheduler_performance_promise_start(async_task_scheduler* s, promise* p);
void		async_task_scheduler_performance_promise_end(async_task_scheduler* s, promise* p);
void		async_task_scheduler_performance_task_start(async_task_scheduler* s, async_task* t);
void		async_task_scheduler_performance_task_end(async_task_scheduler* s, async_task* t);
