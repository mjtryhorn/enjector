/*
* xmemory.h
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

#include <memory.h>
#include <stdbool.h>

#ifdef _MSC_VER
#define bzero(a, b) memset(a, 0x0, b)
#endif

typedef struct memory_allocation_t {
    void* ptr;
    unsigned long size;
    char* filename;
    unsigned int line;
    bool is_resident;
    char* stack_trace;
} memory_allocation;

void*		_xmemory_malloc(unsigned long size, const char* filename, unsigned int line);
#define		xmemory_malloc(size) _xmemory_malloc(size, __FILE__, __LINE__)

#define		xmemory_new(type) (type*) _xmemory_malloc(sizeof(type), __FILE__, __LINE__)
void		xmemory_free(void* ptr);
#define		xmemory_create xmemory_new

size_t		xmemory_report_allocations();

size_t		xmemory_report_fetch(memory_allocation* allocations[], unsigned int max_allocations);

bool		xmemory_report_has_leaks();
void		xmemory_report_print();
void		xmemory_report_clear();
void		xmemory_report_exit_on_leaks();
