/*
* hex.h
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

char*				hex_from_bin(unsigned char* bin, size_t bin_len);
unsigned char*		hex_to_bin(const char* hex, size_t hex_len);
size_t				hex_to_unsigned_integer(const char* hex);
unsigned long long	hex_to_unsigned_long_long(const char* hex);
void				hex_print(const char* prefix, unsigned char* bin, size_t bin_len);
void				hex_free(void* ptr);
//char*			hex_from_radix10(const char* radix10);
