/*
* map.h
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

#include <enjector/core/limits.h>
#include <enjector/core/map.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

map* _map_create(const char* filename, size_t line) {
    map* m = (map*)_xmemory_malloc(sizeof(map), filename, line);
    m->data = (map_item**)_xmemory_malloc(sizeof(map_item*) * LIST_DEFAULT_STREAM_CAPACITY, filename, line);
    m->length = 0;
    m->stream_capacity = LIST_DEFAULT_STREAM_CAPACITY;

    m->alloc_filename = text_clone(filename);
    m->alloc_line = line;

    return m;
}

bool map_set_with_type(map* m, const char* name, const char* type, void* value) {
    assert(m);
    assert(name);
    assert(value);

    // Check if its already set with the same name
    map_item* item = map_get_item(m, name);

    if (item != NULL) {
        item->value = value;
        return true;
    }

    // Check if destination is a stream
    if (m->stream_capacity == 0) {
        printf("map not a stream");
        assert(0);
    }

    // Check if there's enough capacity in the stream, resize otherwise
    const long space_left = (long)(m->stream_capacity - (m->length + 1));

    if (space_left < 0) {
        // Resize
        m->stream_capacity += 1 + LIST_DEFAULT_STREAM_CAPACITY;
        map_item** resized_buffer = (map_item**)_xmemory_malloc(sizeof(map_item*) * m->stream_capacity, m->alloc_filename, m->alloc_line);
        memcpy(resized_buffer, m->data, sizeof(map_item*) * m->length);
        xmemory_free(m->data);
        m->data = resized_buffer;
    }

    // Append
    item = xmemory_new(map_item);
    item->name = text_clone(name);
    item->type = type != NULL ? text_clone(type) : NULL;
    item->value = value;
    m->data[m->length] = item;
    m->length++;

    return true;
}

bool map_set(map* m, const char* name, void* value) {
    return map_set_with_type(m, name, NULL, value);
}

map_item* map_get_item(const map* m, const char* name) {
    assert(m);
    assert(name);

    for (size_t i = 0; i < m->length; i++) {
        map_item* item = m->data[i];

        assert(item);
        assert(item->name);

        if (text_equals(item->name, name)) {
            return item;
        }
    }

    return NULL;
}

map_item* map_get_item_at(const map* m, size_t index) {
    assert(m);
    assert(index < m->length);

    map_item* item = m->data[index];

    return item;
}

bool map_exists(const map* m, const char* name) {
    return map_get_item(m, name) != NULL;
}

void* map_get_value(const map* m, const char* name) {
    map_item* item = map_get_item(m, name);

    if (item == NULL) {
        return NULL;
    }

    return item->value;
}

map_item** map_enumerable(const map* m) {
    assert(m);
    return m->data;
}

size_t map_count(const map* m) {
    assert(m);
    return m->length;
}

void map_clear(map* m) {
    assert(m);

    for (size_t i = 0; i < m->length; i++) {
        map_item* item = m->data[i];
        map_item_free(item);
    }

    m->length = 0;
}

void map_free(map* m) {
    map_clear(m);

    if (m->alloc_filename) xmemory_free(m->alloc_filename);

    xmemory_free(m->data);
    xmemory_free((void*)m);
}

void map_item_free(map_item* item) {
    assert(item);
    assert(item->name);

    if (item->type) {
        xmemory_free(item->type);
    }

    xmemory_free(item->name);
    xmemory_free(item);
}

void map_remove(map* m, const char* name) {
    assert(m);
    assert(name);

    const size_t items_count = map_count(m);

    map_item** items = map_enumerable(m);

    for (size_t i = 0; i < m->length; i++) {
        map_item* item = m->data[i];

        assert(item);
        assert(item->name);

        if (text_equals(item->name, name)) {

            // Remove the slot
            map_item_free(item);

            // If at the end, skip block moving
            if (i != items_count - 1) {
                // Move block
                void* from = &items[i + 1];
                void* to = &items[i];
                const size_t blocks = items_count - 1 - i;

                // Move array pointers along
                memmove(to, from, sizeof(map_item*) * blocks);
            }

            m->length--;
            break;
        }
    }
}

void map_item_dispose(map* m, const char* name) {
    assert(m);
    assert(name);

    const size_t items_count = map_count(m);

    map_item** items = map_enumerable(m);

    for (size_t i = 0; i < m->length; i++) {
        map_item* item = m->data[i];

        assert(item);
        assert(item->name);

        if (text_equals(item->name, name)) {

            if (item->value) {
                xmemory_free(item->value);
            }

            // Remove the slot
            map_item_free(item);

            // If at the end, skip block moving
            if (i != items_count - 1) {

                // Move block
                void* from = &items[i + 1];
                void* to = &items[i];
                const size_t blocks = items_count - 1 - i;

                // Move array pointers along
                memmove(to, from, sizeof(map_item*) * blocks);
            }

            m->length--;
            break;
        }
    }
}

