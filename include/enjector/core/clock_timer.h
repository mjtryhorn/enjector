/*
* clock_timer.h
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

#include <enjector/core/clock.h>

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    clock_time_index    next;
    clock_time_quantity duration;
    size_t              reset_count;
} clock_timer;

void                clock_timer_init(clock_timer* timer, clock_time_quantity intial);
void                clock_timer_reset(clock_timer* timer);
clock_time_quantity clock_timer_value(clock_timer* timer);
bool                clock_timer_has_expired(clock_timer* timer);
