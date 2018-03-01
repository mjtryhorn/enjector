/*
* clock_timer.c
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

#include <enjector/core/clock.h>
#include <enjector/core/clock_timer.h>

#include <assert.h>

void clock_timer_init(clock_timer* timer, clock_time_quantity initial) {
    assert(timer);

    timer->duration = initial;
    timer->reset_count = 0;

    clock_timer_reset(timer);
}

void clock_timer_reset(clock_timer* timer) {
    assert(timer);
    timer->next = clock_now_milliseconds() + (double long)timer->duration;
    timer->reset_count++;
}

bool clock_timer_has_expired(clock_timer* timer) {
    assert(timer);
    return clock_now_milliseconds() > timer->next;
}

clock_time_quantity clock_timer_value(clock_timer* timer) {
    assert(timer);
    return timer->next - clock_now_milliseconds();
}

