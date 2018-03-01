/*
* clock.h
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

typedef long double clock_time_index;
typedef long double clock_time_quantity;

/**
 * Returns the current time in milliseconds since 1900.
 *
 * @returns	The number of seconds including milliseconds.
 */
long double clock_now_milliseconds();

/**
* Returns the current time in seconds since 1900.
*
* @returns	The number of seconds including seconds.
*/
clock_time_index clock_now_seconds();

/**
 * Returns the current UTC date and time stamp.
 *
 * @returns	The UTC time in the format YYYY-MM-DD HH-MM-SS.mm
 */
const char* clock_now_utc();
