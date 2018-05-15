/*
* promise.c
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
#include <enjector/core/xmemory.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

promise* _promise_init(promise* p, const char* name, promise_cb_run run, void* ctx) {
    assert(p);
    assert(run);
    assert(name);

    strncpy_s(p->name, PROMISE_NAME_MAX_LENGTH, name, 1);
    p->state = PromiseStateReady;
    p->error = NULL;
    p->result = NULL;

    p->run = run;
    p->ctx = ctx;

    clock_stopwatch_start(&p->stopwatch);

    return p;
}

void promise_run(promise* p) {
    assert(p);
    assert(p->run);
    assert(p->state == PromiseStateReady || p->state == PromiseStateRunning || p->state == PromiseStateDefer);

    p->state = PromiseStateRunning;
    p->run(p, p->ctx);
}

/**
 * A promise can generate an error using this.
 */
void _promise_reject(promise* p, const char* ex) {
    assert(p);
    assert(ex);

    p->error = ex;
    p->state = PromiseStateError;
}

/**
 * A promise can defer its own execution and be called again in the next cycle.
 */
void _promise_defer(promise* p) {
    assert(p);

    p->state = PromiseStateDefer;
}

/**
 * The promise completes successfully with a resultant object.
 */
void _promise_complete(promise* p, void* value) {
    assert(p);

    p->result = value;
    p->state = PromiseStateResult;
}
