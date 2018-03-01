/*
* test.h
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

#include <stddef.h>

typedef struct string_buffer_t {
    char* data;
    unsigned long length;
    unsigned long stream_capacity;

    const char*  _allocated_filename;
    unsigned int _allocated_line;
} string_buffer;

string_buffer*	_string_buffer_create_empty(const char* filename, unsigned int line);
#define			string_buffer_create_empty() _string_buffer_create_empty(__FILE__, __LINE__)

string_buffer*	_string_buffer_create(const char* data, size_t length, const char* filename, unsigned int line);
#define			string_buffer_create(data, length) _string_buffer_create(data, length, __FILE__, __LINE__)

void			string_buffer_append(string_buffer* sb, const char* data);
void			string_buffer_append_length(string_buffer* sb, const char* data, unsigned int length);
void			string_buffer_append_format(string_buffer* sb, const char* format, ...);
const char*		string_buffer_get(const string_buffer* sb);
char*			string_buffer_move(string_buffer* sb);
unsigned long	string_buffer_length(const string_buffer* sb);
void			string_buffer_clear(string_buffer* sb);
void			string_buffer_free(string_buffer* sb);
void			string_buffer_free_not_data(string_buffer* sb);
