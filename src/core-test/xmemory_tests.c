/*
* xmemory_tests.c
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

#ifdef _DEBUG_MEMORY

#include "xmemory_tests.h"

#include <enjector/core/xmemory.h>

#define XMEMORY_MAX_ALLOCATIONS	1024

static void should_successfully_allocate_memory_and_add_to_tracking() {
    memory_allocation* allocations[XMEMORY_MAX_ALLOCATIONS];
    char* new_memory1 = xmemory_malloc(sizeof(char*));

    const size_t allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE_FATAL(allocations_total, 1);

    TEST_ASSERT_EQUAL_PTR(allocations[0]->ptr, new_memory1);
    TEST_ASSERT_EQUAL_SIZE(allocations[0]->size, sizeof(char*));
    TEST_ASSERT_PTR_NOT_NULL(allocations[0]->filename);
    TEST_ASSERT_NOT_EQUAL_SIZE(0, allocations[0]->line);
    TEST_ASSERT_EQUAL_SIZE(allocations[0]->is_resident, true);

    xmemory_report_clear();
}

static void should_successfully_add_to_an_existing_allocation_list() {
    memory_allocation* allocations[XMEMORY_MAX_ALLOCATIONS];

    char* new_memory1 = xmemory_malloc(sizeof(char*));
    size_t allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE_FATAL(allocations_total, 1);

    char* new_memory2 = xmemory_malloc(sizeof(char*));
    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE(allocations_total, 2);

    TEST_ASSERT_EQUAL_PTR(allocations[0]->ptr, new_memory1);
    TEST_ASSERT_EQUAL_PTR(allocations[1]->ptr, new_memory2);
    TEST_ASSERT_PTR_NOT_NULL(allocations[0]->filename);
    TEST_ASSERT_NOT_EQUAL_SIZE(0, allocations[0]->line);
    TEST_ASSERT_EQUAL_BOOL(allocations[0]->is_resident, true);
    TEST_ASSERT_PTR_NOT_NULL(allocations[1]->filename);
    TEST_ASSERT_NOT_EQUAL_SIZE(0, allocations[1]->line);
    TEST_ASSERT_EQUAL_BOOL(allocations[1]->is_resident, true);

    xmemory_report_clear();
}

static void should_successfully_free_memory() {
    memory_allocation* allocations[XMEMORY_MAX_ALLOCATIONS];

    char* new_memory = xmemory_malloc(sizeof(char*));

    size_t allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);

    xmemory_free(new_memory);

    TEST_ASSERT_EQUAL_BOOL(allocations[0]->is_resident, false);

    xmemory_report_clear();
}

static void should_successfully_add_and_free_multiple_items_to_an_allocation_list() {
    memory_allocation* allocations[XMEMORY_MAX_ALLOCATIONS];

    char* new_memory1 = xmemory_malloc(sizeof(char*));
    size_t allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE_FATAL(allocations_total, 1);

    char* new_memory2 = xmemory_malloc(sizeof(char*));
    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE_FATAL(allocations_total, 2);

    char* new_memory3 = xmemory_malloc(sizeof(char*));
    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE_FATAL(allocations_total, 3);

    TEST_ASSERT_EQUAL_PTR(allocations[0]->ptr, new_memory1);
    TEST_ASSERT_PTR_NOT_NULL(allocations[0]->filename);
    TEST_ASSERT_NOT_EQUAL_SIZE(0, allocations[0]->line);
    TEST_ASSERT_EQUAL_BOOL(allocations[0]->is_resident, true);
    TEST_ASSERT_EQUAL_PTR(allocations[1]->ptr, new_memory2);
    TEST_ASSERT_PTR_NOT_NULL(allocations[1]->filename);
    TEST_ASSERT_NOT_EQUAL_SIZE(0, allocations[1]->line);
    TEST_ASSERT_EQUAL_BOOL(allocations[1]->is_resident, true);
    TEST_ASSERT_EQUAL_PTR(allocations[2]->ptr, new_memory3);
    TEST_ASSERT_PTR_NOT_NULL(allocations[2]->filename);
    TEST_ASSERT_NOT_EQUAL_SIZE(0, allocations[2]->line);
    TEST_ASSERT_EQUAL_BOOL(allocations[2]->is_resident, true);

    xmemory_free(new_memory1);
    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE(allocations_total, 2);
    TEST_ASSERT_EQUAL_BOOL(allocations[0]->is_resident, true);
    TEST_ASSERT_EQUAL_BOOL(allocations[1]->is_resident, true);

    xmemory_free(new_memory2);
    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE(allocations_total, 1);
    TEST_ASSERT_EQUAL_BOOL(allocations[0]->is_resident, true);

    xmemory_free(new_memory3);
    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE(allocations_total, 0);

    xmemory_report_clear();
}

static void should_successfully_report_leaks() {
    char* new_memory = xmemory_malloc(sizeof(char*));
    bool has_leaks = xmemory_report_has_leaks();

    TEST_ASSERT_TRUE(has_leaks);

    xmemory_free(new_memory);

    has_leaks = xmemory_report_has_leaks();

    TEST_ASSERT_FALSE(has_leaks);

    xmemory_report_clear();
}

static void should_successfully_report_multiple_leaks() {
    char* new_memory1 = xmemory_malloc(sizeof(char*));
    char* new_memory2 = xmemory_malloc(sizeof(char*));

    bool has_leaks = xmemory_report_has_leaks();
    TEST_ASSERT_TRUE(has_leaks);

    xmemory_free(new_memory1);

    has_leaks = xmemory_report_has_leaks();
    TEST_ASSERT_TRUE(has_leaks);

    xmemory_free(new_memory2);
    has_leaks = xmemory_report_has_leaks();
    TEST_ASSERT_FALSE(has_leaks);

    xmemory_report_clear();
}

static void should_successfully_clear_memory_report() {
    memory_allocation* allocations[XMEMORY_MAX_ALLOCATIONS];

    char* new_memory1 = xmemory_malloc(sizeof(char*));
    char* new_memory2 = xmemory_malloc(sizeof(char*));

    xmemory_report_clear();

    const size_t allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);

    TEST_ASSERT_EQUAL_SIZE(allocations_total, 0);
}

static void should_successfully_duplicate_string() {
    memory_allocation* allocations[XMEMORY_MAX_ALLOCATIONS];

    const char* message = "Hello World";

    char* message_copy = xmemory_strdup(message);
    TEST_ASSERT_EQUAL_STRING(message, message_copy);

    size_t allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE(allocations_total, 1);

    xmemory_free(message_copy);

    allocations_total = xmemory_report_fetch(allocations, XMEMORY_MAX_ALLOCATIONS);
    TEST_ASSERT_EQUAL_SIZE(allocations_total, 0);

    xmemory_report_clear();
}

test xmemory_tests[] = {
    { "should_successfully_allocate_memory_and_add_to_tracking", should_successfully_allocate_memory_and_add_to_tracking },
    { "should_successfully_add_to_an_existing_allocation_list", should_successfully_add_to_an_existing_allocation_list },
    { "should_successfully_free_memory", should_successfully_free_memory },
    { "should_successfully_add_and_free_multiple_items_to_an_allocation_list", should_successfully_add_and_free_multiple_items_to_an_allocation_list },
    { "should_successfully_report_leaks", should_successfully_report_leaks },
    { "should_successfully_report_multiple_leaks", should_successfully_report_multiple_leaks },
    { "should_successfully_clear_memory_report", should_successfully_clear_memory_report },
    { "should_successfully_duplicate_string", should_successfully_duplicate_string },
    TEST_END
};

#endif