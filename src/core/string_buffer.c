/*
* string_buffer.c
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
#include <enjector/core/string_buffer.h>
#include <enjector/core/xmemory.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

string_buffer* _string_buffer_create_empty(const char* filename, unsigned int line) {
    assert(filename);

    string_buffer* sb = _xmemory_malloc(sizeof(string_buffer), filename, line);
    sb->_allocated_filename = filename;
    sb->_allocated_line = line;

    sb->data = _xmemory_malloc(STRING_BUFFER_DEFAULT_STREAM_CAPACITY, filename, line);
    sb->length = 0;
    sb->stream_capacity = STRING_BUFFER_DEFAULT_STREAM_CAPACITY;

    return sb;
}

string_buffer* _string_buffer_create(const char* data, size_t length, const char* filename, unsigned int line) {
    assert(data);

    string_buffer* sb = _xmemory_malloc(sizeof(string_buffer), filename, line);
    sb->_allocated_filename = filename;
    sb->_allocated_line = line;

    sb->data = _xmemory_malloc(length + STRING_BUFFER_DEFAULT_STREAM_CAPACITY, filename, line);
    memcpy(sb->data, data, length);

    sb->length = length;
    sb->stream_capacity = length + STRING_BUFFER_DEFAULT_STREAM_CAPACITY;

    return sb;
}

void string_buffer_append(string_buffer* sb, const char* data) {
    assert(sb);
    assert(data);

    // Check if destination is a stream
    if(sb->stream_capacity == 0) {
        printf("string_buffer not a stream");
        assert(0);
    }

    // Check if there's enough capacity in the stream, resize otherwise
    unsigned long data_len = strlen(data);
    long space_left = sb->stream_capacity - (sb->length + data_len) - 1; // -1 for NULL

    if(space_left < 0) {
        // Resize
        sb->stream_capacity += data_len + STRING_BUFFER_DEFAULT_STREAM_CAPACITY;
        char* resized_buffer = (char*) _xmemory_malloc(sb->stream_capacity, sb->_allocated_filename, sb->_allocated_line);
        memcpy(resized_buffer, sb->data, sb->length);
        xmemory_free(sb->data);
        sb->data = resized_buffer;
    }

    // Append
    memcpy(&sb->data[sb->length], data, data_len);
    sb->length += data_len;
    sb->data[sb->length] = 0;
}

void string_buffer_append_length(string_buffer* sb, const char* data, unsigned int length) {
    assert(sb);
    assert(data);

    // Check if destination is a stream
    if(sb->stream_capacity == 0) {
        printf("string_buffer not a stream");
        assert(0);
    }

    // Check if there's enough capacity in the stream, resize otherwise
    unsigned long data_len = strlen(data);
    assert(length <= data_len);
    data_len = length;	// Use the specified length

    long space_left = sb->stream_capacity - (sb->length + data_len) - 1; // -1 for NULL

    if(space_left < 0) {
        // Resize
        sb->stream_capacity += data_len + STRING_BUFFER_DEFAULT_STREAM_CAPACITY;
        //resized_buffer = (char*)_xmemory_malloc(sb->stream_capacity, sb->_allocated_filename, sb->_allocated_line);
        //memcpy(resized_buffer, sb->data, sb->length);
        //xmemory_free(sb->data);

        //sb->data = xmemory_realloc(sb->data, sb->stream_capacity);
        char* resized_buffer = (char*)_xmemory_malloc(sb->stream_capacity, sb->_allocated_filename, sb->_allocated_line);
        memcpy(resized_buffer, sb->data, sb->length);
        xmemory_free(sb->data);
        sb->data = resized_buffer;

    }

    // Append
    memcpy(&sb->data[sb->length], data, data_len);
    sb->length += data_len;
    sb->data[sb->length] = 0;
}

void string_buffer_append_format(string_buffer* sb, const char* format, ...) {
    assert(sb);
    assert(format);

    char data[2048];
    va_list args;

    va_start(args, format);
    vsnprintf(data, sizeof(data), format, args);
    va_end(args);

    string_buffer_append(sb, data);
}

const char* string_buffer_get(const string_buffer* sb) {
    assert(sb);

    return sb->data;
}

// Unwraps and moves the ownership of the data
char* string_buffer_move(string_buffer* sb) {
    assert(sb);

    char* data = sb->data;
    sb->data = NULL;

    string_buffer_free(sb);

    return data;
}

unsigned long string_buffer_length(const string_buffer* sb) {
    assert(sb);

    return sb->length;
}

void string_buffer_clear(string_buffer* sb) {
    assert(sb);

    sb->length = 0;
}

void string_buffer_free(string_buffer* sb) {
    assert(sb);
    string_buffer_clear(sb);

    // It may have been get_move
    if(sb->data) xmemory_free(sb->data);

    xmemory_free(sb);
}

void string_buffer_free_not_data(string_buffer* sb) {
    assert(sb);
    xmemory_free(sb);
}
