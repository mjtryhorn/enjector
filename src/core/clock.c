/*
* clock_win32.c
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

#include <stdio.h>
#include <assert.h>

#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <sys/timeb.h>

int gettimeofday(struct timeval* t, void* timezone) {
    assert(t);

    struct _timeb timebuffer;
    _ftime_s(&timebuffer);
    t->tv_sec = (long)timebuffer.time;
    t->tv_usec = 1000 * timebuffer.millitm;
    return 0;
}

#else
#include <sys/time.h>
#endif

/**
 * Returns the current time in milliseconds since 1900.
 *
 * @returns	The number of seconds including millisecond.
 */
clock_time_index clock_now_milliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    const clock_time_index total = (clock_time_index)(tv.tv_sec * 1e3 + tv.tv_usec / 1e3);

    return total;
}

/**
* Returns the current time in seconds since 1900.
*
* @returns	The number of seconds including milliseconds.
*/
clock_time_index clock_now_seconds() {
    return (clock_time_index)(clock_now_milliseconds() / 1e3);
}

/**
 * Returns the current UTC date and time stamp.
 *
 * @returns	The UTC time in the format YYYY-MM-DD HH:MM:SS.mmm
 */

const char* clock_now_utc() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Convert and get date-time down to the seconds
    time_t t = tv.tv_sec;
    char buffer[60];
    struct tm timestamp = {0, 0, 0, 0, 0, 0, 0, 0, 0};
#ifdef _WIN32
    localtime_s(&timestamp, &t);
#else
    localtime_r(&timestamp, &t);
#endif

    size_t len = strftime(buffer, 60, "%Y-%m-%d %H:%M:%S", &timestamp);

    // Append the milliseconds
    static char bufferWithMilliseconds[80];
    snprintf(bufferWithMilliseconds, 80, "%s.%-2ld", buffer, tv.tv_usec / 1000);

    return bufferWithMilliseconds;
}
