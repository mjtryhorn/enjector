/*
* clock_stopwatch.h
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

typedef struct clock_stopwatch_t {
    clock_time_index start;
    clock_time_index end;
    clock_time_quantity elapsed;
} clock_stopwatch;

/**
 * Creates and starts the stopwatch. The stopwatch object is initialised
 * with the start time. A call to clock_stopwatch_end will stop the
 * stopwatch.
 *
 * @param stopwatch		Pointer to the stopwatch object that will hold
 *						the timing details.
 */
void clock_stopwatch_start(clock_stopwatch* stopwatch);

/**
 * Stops the stopwatch.
 *
 * @param stopwatch		Pointer to a previous started stopwatch object
 *						that is holding the timing details.
 */
clock_time_quantity clock_stopwatch_end(clock_stopwatch* stopwatch);

/**
* Returns the stopwatch's elapsed time.
*
* @param stopwatch		Pointer to a previous started and ended stopwatch object
*						that is holding the timing details.
*/
clock_time_quantity clock_stopwatch_elapsed(clock_stopwatch* stopwatch);

/**
* Returns the stopwatch's elapsed time in HH:mm:ss.ms format
*
* @param stopwatch		Pointer to a previous started and ended stopwatch object
*						that is holding the timing details.
*/
const char* clock_stopwatch_elapsed_time(clock_stopwatch* stopwatch);

/**
 * Prints the stopwatch's elapsed time.
 *
 * @param stopwatch		Pointer to a previous started and ended stopwatch object
 *						that is holding the timing details.
 */
void clock_stopwatch_elapsed_print(clock_stopwatch* stopwatch);

/**
 * Calculate the rate based on the stopwatch's elapsed time.
 *
 * @param stopwatch		Pointer to a previous started and ended stopwatch object
 *						that is holding the timing details.
 * @param count
 */
size_t clock_stopwatch_rate_calculate(clock_stopwatch* stopwatch, size_t count);

/**
 * Prints the rate based on the stopwatch's elapsed time.
 *
 * @param stopwatch		Pointer to a previous started and ended stopwatch object
 *						that is holding the timing details.
 * @param name
 * @param count
 */
void clock_stopwatch_rate_print(clock_stopwatch* stopwatch, const char* name, int count);
