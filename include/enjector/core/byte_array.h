/*
* byte_array.h
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

#ifdef _WIN32
#include <inttypes.h>
#else
#include <stdlib.h>
#endif

typedef struct byte_array_t {
    unsigned char* data;
    size_t length;
    size_t stream_capacity;
} byte_array;

#define		byte_array_create(data, length) _byte_array_create(data, length, __FILE__, __LINE__)
byte_array* _byte_array_create(const unsigned char* data, size_t length, const char* filename, unsigned int line);

#define		byte_array_create_empty(length) _byte_array_create_empty(length, __FILE__, __LINE__)
byte_array* _byte_array_create_empty(size_t length, const char* filename, unsigned int line);

#define		byte_array_create_stream() _byte_array_create_stream(__FILE__, __LINE__)
byte_array* _byte_array_create_stream(const char* filename, unsigned int line);

byte_array*	_byte_array_create_from_hex(const char* hex, const char* filename, unsigned int line);
#define		byte_array_create_from_hex(hex) _byte_array_create_from_hex(hex, __FILE__, __LINE__)

//byte_array*	_byte_array_create_from_base64(const char* base64, const char* filename, unsigned int line);
//#define		byte_array_create_from_base64(hex) _byte_array_create_from_hex(base64, __FILE__, __LINE__)

void		byte_array_free(byte_array* ba);

void		byte_array_append(byte_array* dst, byte_array* src, size_t offset, size_t length);
void		byte_array_copy(byte_array* dst, unsigned int dst_index, byte_array* src, unsigned int src_index, size_t length);
void		byte_array_append_byte(byte_array* dst, unsigned char b);
void		byte_array_append_raw(byte_array* dst, const unsigned char* data, size_t offset, size_t length);

int			byte_array_is_equal(byte_array* a, byte_array* b);
int			byte_array_is_equal_raw(byte_array* a, unsigned char* b_data, size_t b_length);

void	    byte_array_print(const char* prefix, byte_array* ba);

void		byte_array_file_write_hex(const char* filename, byte_array* ba);

char*		byte_array_to_hex(byte_array* ba);
char*		byte_array_to_base64(byte_array* ba);

byte_array* byte_array_clone(byte_array* src);

void		byte_array_reverse(byte_array* ba);

byte_array*	byte_array_rotate_left_with_overflow(byte_array* source);
byte_array*	byte_array_xor(byte_array* source1, byte_array* source2);
