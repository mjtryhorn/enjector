/*
* text.h
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

#include <enjector/core/list.h>

char*	    _text_clone(const char* str, const char* filename, unsigned int line);
#define     text_clone(str) _text_clone(str, __FILE__, __LINE__)

char*	    _text_clone_length(const char* str, size_t str_len, const char* filename, unsigned int line);
#define     text_clone_length(str, str_len) _text_clone_length(str, str_len, __FILE__, __LINE__)

void	    text_free(char* s);

char*	    text_copy(char* dest, const char* src);
char*	    text_copy_length(char* dest, const char* src, size_t len);

long	    text_length(const char* str);

int		    text_compare(const char* s1, const char* s2);
int		    text_compare_nocase(const char* p1, const char* p2);

void	    text_replace_inplace(char* source, char old_value, char new_value);
char*	    _text_replace(const char* source, const char* old_value, const char* new_value, const char* filename, unsigned int line);
#define     text_replace(source, old_value, new_value) _text_replace(source, old_value, new_value, __FILE__, __LINE__)

char*	    text_append(char* target, unsigned int target_size, const char* source);

char*	    _text_trim_left(const char* s, const char* filename, unsigned int line);
#define     text_trim_left(s) _text_trim_left(s, __FILE__, __LINE__)

char*       _text_trim_right(const char* s, const char* filename, unsigned int line);
#define     text_trim_right(s) _text_trim_right(s, __FILE__, __LINE__)

char*	    _text_trim(const char* s, const char* filename, unsigned int line);
#define     text_trim(s) _text_trim(s, __FILE__, __LINE__)

bool        text_is_null_or_empty(const char* s);

char*	    _text_to_lower(const char* s, const char* filename, unsigned int line);
#define     text_to_lower(s) _text_to_lower(s, __FILE__, __LINE__)

char*	    _text_to_upper(const char* s, const char* filename, unsigned int line);
#define     text_to_upper(s) _text_to_upper(s, __FILE__, __LINE__)

const char* text_last_index_of(const char* s, char c);
bool        text_equals(const char* s1, const char* s2);
bool        text_starts_with(const char* s1, const char* s2);

char*       _text_format(const char* filename, unsigned int line, const char* format, ...);
#define     text_format(format, ...) _text_format(__FILE__, __LINE__, format, __VA_ARGS__)

list*       _text_split(const char* str, char split_char, const char* filename, unsigned int line);
#define     text_split(str, split_char) _text_split(str, split_char, __FILE__, __LINE__)

#define     text_concat(a, b) a ## b
