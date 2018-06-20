/*
* promise.h
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

#include <enjector/core/limits.h>
#include <enjector/core/clock_stopwatch.h>

typedef enum PromiseState_t {
    PromiseStateDefault = 0,
    PromiseStateReady = 1,      // Hasn't been run
    PromiseStateRunning = 2,    // Is Running
    PromiseStateDefer = 3,      // Call the promise again (polling)
    PromiseStateError = 4,      // Promise errored due to reject() being called
    PromiseStateResult = 5,     // Promise has a result due to fulfil() being called
    //	PromiseStateFinish = 6      // Promise has completed and not used
} PromiseState;

typedef void (*fulfil_cb)(void*);
typedef void (*defer_cb)();
typedef void (*reject_cb)(const char*);

typedef struct promise_t promise;
typedef void (*promise_cb_run)(promise* promise, void* ctx);

typedef struct promise_t {
    char name[PROMISE_NAME_MAX_LENGTH];

    PromiseState state;

    const char* error;    // We have string to ensure we have a copy of the error message
    void* result;       // This is an opaque object that is casted back to the required type using decltype

    fulfil_cb fulfil;
    defer_cb defer;
    reject_cb reject;

    promise_cb_run run;
    void** ctx;

    clock_stopwatch stopwatch;
} promise;

/**
 * A promise can generate an error using this.
 */
void _promise_reject(promise* p, const char* ex);

/**
 * A promise can defer its own execution and be called again in the next cycle.
 */
void _promise_defer(promise* p);

/**
 * The promise completes successfully with a resultant object.
 */
void _promise_complete(promise* p, void* value);

#define promise_complete(v) _promise_complete(_promise, (void*) v);
#define promise_reject(error) _promise_reject(_promise, error);
#define promise_defer() _promise_defer(_promise); return;

#define promise_arg(fn_name, ct, c) void fn_name(promise* _promise, ct c)
#define promise_noarg(fn_name) void fn_name(promise* _promise, void* _ctx)

#ifdef _WIN32

#pragma warning(disable:4002)
#pragma warning(disable:4003)
#define PROMISE_SELMACRO_IMPL(_1, _2, _3, N,...) N
#define PROMISE_SELMACRO_IMPL_(tuple) PROMISE_SELMACRO_IMPL tuple
#define promise(...) PROMISE_SELMACRO_IMPL_((__VA_ARGS__, promise_arg(__VA_ARGS__), NULL, promise_noarg(__VA_ARGS__)))

#else

#define PROMISE_MACRO_CHOOSER_GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define PROMISE_MACRO_CHOOSER(...) PROMISE_MACRO_CHOOSER_GET_4TH_ARG(__VA_ARGS__, promise_arg, NULL, promise_noarg)
#define promise(...) PROMISE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#endif

promise*	_promise_init(promise* p, const char* name, promise_cb_run run, void* ctx);
#define		promise_init(p, fn, ctx) _promise_init(p, #fn, fn, ctx);

void		promise_run(promise* p);

#define		promise_result(p, result_type) (result_type) p.result
