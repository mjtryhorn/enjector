/*
* clock_stopwatch.c
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
#include <enjector/core/clock_stopwatch.h>

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

/**
 * Creates and starts the stopwatch. The stopwatch object is initialised
 * with the start time. A call to clock_stopwatch_end will stop the
 * stopwatch.
 *
 * @param stopwatch		Pointer to the stopwatch object that will hold
 *						the timing details.
 */
void clock_stopwatch_start(clock_stopwatch* stopwatch) {
    assert(stopwatch);

    stopwatch->start = clock_now_milliseconds();
    stopwatch->end = 0;
    stopwatch->elapsed = 0;
}

/**
 * Stops the stopwatch.
 *
 * @param stopwatch		Pointer to a previous started stopwatch object
 *						that is holding the timing details.
 */
clock_time_quantity clock_stopwatch_end(clock_stopwatch* stopwatch) {
    assert(stopwatch);

    stopwatch->end = clock_now_milliseconds();
    stopwatch->elapsed = stopwatch->end - stopwatch->start;

    return stopwatch->elapsed;
}

/**
* Returns the stopwatch's elapsed time.
*
* @param stopwatch		Pointer to a previous started and ended stopwatch object
*						that is holding the timing details.
*/
clock_time_quantity clock_stopwatch_elapsed(clock_stopwatch* stopwatch) {
    assert(stopwatch);

    // If the stopwatch hasn't finished then update the elapsed
    if(stopwatch->end == 0) {
        clock_time_quantity now = clock_now_milliseconds();
        stopwatch->elapsed = now - stopwatch->start;
    }

    return stopwatch->elapsed;
}

/**
* Returns the stopwatch's elapsed time in HH:mm:ss.ms format
*
* @param stopwatch		Pointer to a previous started and ended stopwatch object
*						that is holding the timing details.
*/
const char* clock_stopwatch_elapsed_time(clock_stopwatch* stopwatch) {
    assert(stopwatch);

    long total = (long) clock_stopwatch_elapsed(stopwatch) / (long) 1e3;

    int days = total / 86400;
    int hours = (total / 3600) - (days * 24);
    int mins = (total / 60) - (days * 1440) - (hours * 60);

    static char buffer[120];
    snprintf(buffer, sizeof(buffer), "%d days, %d hours, %d minutes, %ld seconds",
             days, hours, mins, total % 60);

    return buffer;
}

/**
 * Prints the stopwatch's elapsed time.
 *
 * @param stopwatch		Pointer to a previous started and ended stopwatch object
 *						that is holding the timing details.
 */
void clock_stopwatch_elapsed_print(clock_stopwatch* stopwatch) {
    assert(stopwatch);

    printf("Elapsed time: %f ms\n", (float)(stopwatch->elapsed));
}

/**
 * Calculate the rate based on the stopwatch's elapsed time.
 *
 * @param stopwatch		Pointer to a previous started and ended stopwatch object
 *						that is holding the timing details.
 */
double clock_stopwatch_rate_calculate(clock_stopwatch* stopwatch, int count) {
    assert(stopwatch);

    if(count <= 0) {
        return 0;
    }

	// TODO: avoid potential divide by 0
    return (double)(count / (stopwatch->elapsed / 1000));
}

/**
 * Prints the rate based on the stopwatch's elapsed time.
 *
 * @param stopwatch		Pointer to a previous started and ended stopwatch object
 *						that is holding the timing details.
 */
void clock_stopwatch_rate_print(clock_stopwatch* stopwatch, const char* name, int count) {
    assert(stopwatch);

    if(count <= 0) {
        return;
    }

    double rate = clock_stopwatch_rate_calculate(stopwatch, count);
    printf("%s rate: %f per second\n", name, rate);
}
