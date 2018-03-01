/*
 * xmemory.c
 *
 * Copyright 2004-2019 Enjector Software, Ltd.
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

#include <enjector/core/limits.h>
#include <enjector/core/xmemory.h>

#ifdef _DEBUG
#define TRACK_MEMORY
//#define TRACK_MEMORY_STACK_TRACE
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef TRACK_MEMORY
static memory_allocation memory_allocation_tracking[XMEMORY_MAX_TRACKING_ALLOCATIONS];
static unsigned int memory_allocation_tracking_count = 0;
#endif

static size_t memory_allocation_count = 0;

size_t xmemory_report_allocations() {
    return memory_allocation_count;
}

#ifdef TRACK_MEMORY

void _xmemory_report_track(void* ptr, unsigned long size, const char* filename, unsigned int line) {
    assert(ptr);

    memory_allocation_count++;

    if (memory_allocation_tracking_count == XMEMORY_MAX_TRACKING_ALLOCATIONS) {
        fprintf(stderr, "_xmemory_malloc: not enough tracking allocations. Increase MAX_TRACKING_ALLOCATIONS\n");
        exit(1);
    }

    // Find a unsed slot
    memory_allocation * allocation = NULL;

    for (unsigned int i = 0; i < memory_allocation_tracking_count; i++) {
        memory_allocation * old_allocation = &memory_allocation_tracking[i];

        if (old_allocation->is_resident == false) {
            allocation = old_allocation;
            break;
        }
    }

    if (allocation == NULL) {
        allocation = &memory_allocation_tracking[memory_allocation_tracking_count];
        memory_allocation_tracking_count++;
    }

    allocation->ptr = ptr;
    allocation->size = size;
    allocation->line = line;
    allocation->is_resident = true;
    allocation->filename = _strdup(filename);

#ifdef TRACK_MEMORY_STACK_TRACE
    allocation->stack_trace = system_stack_trace();
#endif
}

void _xmemory_report_untrack(void* ptr) {
    assert(ptr);

    // Remove from tracking
    for (unsigned int i = 0; i < memory_allocation_tracking_count; i++) {
        memory_allocation * allocation = &memory_allocation_tracking[i];

        // Check if the memory has already been cleared.
        if (allocation->ptr == ptr && allocation->is_resident) {
            allocation->is_resident = false;
            free(allocation->filename);
            break;
        }
    }
}

#endif

void* _xmemory_malloc(unsigned long size, const char* filename, unsigned int line) {
    assert(size > 0);
    assert(filename);

    void * ptr = calloc(1, size);

#ifdef TRACK_MEMORY
    _xmemory_report_track(ptr, size, filename, line);
#endif

    return ptr;
}

void xmemory_free(void* ptr) {
    assert(ptr);

#ifdef TRACK_MEMORY

    // Remove from tracking
    bool was_tracked = false;

    for (unsigned int i = 0; i < memory_allocation_tracking_count; i++) {
        memory_allocation * allocation = &memory_allocation_tracking[i];

        // Check if the memory has already been cleared.
        if (allocation->ptr == ptr)
            if (allocation->is_resident) {
                allocation->is_resident = false;
                free(allocation->filename);

#ifdef TRACK_MEMORY_STACK_TRACE
                free(allocation->stack_trace);
#endif
                was_tracked = true;
                break;
            }
    }

    //	assert(was_tracked);
    memory_allocation_count--;

#endif
}

size_t xmemory_report_fetch(memory_allocation* allocations[], unsigned int max_allocations) {
    assert(allocations);
    assert(max_allocations > 0);

#ifdef TRACK_MEMORY
    unsigned int total_allocations;
    unsigned int allocations_count = 0;

    assert(allocations);
    assert(max_allocations > 0);
    assert(memory_allocation_tracking);

    total_allocations = memory_allocation_tracking_count;

    // Is there enough space for the report?
    assert(total_allocations < max_allocations);

    for (unsigned int i = 0; i < total_allocations; i++) {
        // Get allocation item
        memory_allocation * allocation = &memory_allocation_tracking[i];

        if (allocation->is_resident) {
            // Add to allocations list
            allocations[allocations_count] = allocation;
            allocations_count++;
        }
    }

    return allocations_count;
#else
    return 0;
#endif
}

bool xmemory_report_has_leaks() {
#ifdef TRACK_MEMORY
    bool has_leaks = false;

    // If there's no allocations, then there's no leaks to report
    if (!memory_allocation_tracking) {
        return true;
    }

    for (unsigned int i = 0; i < memory_allocation_tracking_count; i++) {
        memory_allocation * allocation = &memory_allocation_tracking[i];

        if (allocation->is_resident) {
            has_leaks = true;
            break;
        }
    }

    return has_leaks;
#else
    return false;
#endif
}

void xmemory_report_print() {
#ifdef TRACK_MEMORY
    int total_leaks = 0;

    if (!memory_allocation_tracking) {
        return;
    }

    for (unsigned int i = 0; i < memory_allocation_tracking_count; i++) {
        memory_allocation * allocation = &memory_allocation_tracking[i];

        if (allocation->is_resident) {
            printf("%s:%d (Size:=%lu): %s\n", allocation->filename, allocation->line, allocation->size, (char*)allocation->ptr);
            total_leaks++;
        }
    }

    printf("Total number of allocations: %d \n", memory_allocation_tracking_count);
    printf("Total number of leaks: %d \n", total_leaks);
#endif
}

void xmemory_report_clear() {
#ifdef TRACK_MEMORY

    if (!memory_allocation_tracking) {
        return;
    }

    for (unsigned int i = 0; i < memory_allocation_tracking_count; i++) {
        memory_allocation * allocation = &memory_allocation_tracking[i];

        if (allocation->is_resident) {
            allocation->is_resident = false;
            free(allocation->filename);
        }
    }

    memory_allocation_tracking_count = 0;
#endif
}

void xmemory_report_exit_on_leaks() {
#ifdef TRACK_MEMORY

    if (xmemory_report_has_leaks()) {
        printf("\n\n");
        printf("=================================================================\n");
        printf("Memory Leaks Detected\n");
        printf("=================================================================\n");
        xmemory_report_print();
        exit(1);
    }

    xmemory_report_clear();
#endif
}
