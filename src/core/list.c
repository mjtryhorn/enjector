/*
* list.c
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
#include <enjector/core/list.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

list* _list_create(const char* filename, unsigned int line) {
    list* l = _xmemory_malloc(sizeof(list), filename, line);
    l->data = (list_item**)_xmemory_malloc(sizeof(list_item*) * LIST_DEFAULT_STREAM_CAPACITY, filename, line);
    l->length = 0;
    l->stream_capacity = LIST_DEFAULT_STREAM_CAPACITY;

    l->alloc_filename = text_clone(filename);
    l->alloc_line = line;

    return l;
}

void list_add_with_type(list* l, const char* type, void* value) {
    assert(l);
    assert(value);

    // Check if destination is a stream
    if(l->stream_capacity == 0) {
        printf("list not a stream");
        assert(0);
    }

    // Check if there's enough capacity in the stream, resize otherwise
    int space_left = l->stream_capacity - (l->length + 1);

    if(space_left < 0) {
        // Resize
        l->stream_capacity += 1 + LIST_DEFAULT_STREAM_CAPACITY;
        list_item** resized_buffer = (list_item**)_xmemory_malloc(sizeof(list_item*) * l->stream_capacity, l->alloc_filename, l->alloc_line);
        memmove(resized_buffer, l->data, sizeof(list_item*) * l->length);
        xmemory_free(l->data);
        l->data = resized_buffer;
    }

    // Append
    list_item* item = (list_item*)xmemory_new(list_item);
    item->type = type ? text_clone(type) : NULL;
    item->value = value;
    l->data[l->length] = item;
    l->length++;
}

void list_add(list* l, void* value) {
    list_add_with_type(l, NULL, value);
}

void list_item_free(list_item* item) {
    assert(item);

    if(item->type) xmemory_free(item->type);

    xmemory_free(item);
}

list_item** list_enumerable(const list* l) {
    assert(l);

    return l->data;
}

void list_remove_item(list* l, list_item* item) {
    assert(l);
    assert(item);
    assert(l->length > 0);

    list_item** items = list_enumerable(l);
    const size_t items_count = list_count(l);

    for(unsigned int i = 0; i < items_count; i++) {
        list_item* p = items[i];

        if(p == item) {
            // Remove the slot
            list_item_free(p);

            // If at the end, skip block moving
            if(i != items_count - 1) {
                // Move block
                void* from = &items[i + 1];
                void* to = &items[i];
                const size_t blocks = items_count - 1 - i;

                // Move array pointers along
                memmove(to, from, sizeof(list_item*) * blocks);
            }

            l->length--;
            break;
        }
    }
}

bool list_remove_item_by_value(list* l, void* value) {
    assert(l);
    assert(value);
    assert(l->length > 0);

    list_item** items = list_enumerable(l);
    const size_t items_count = list_count(l);

    for(unsigned int i = 0; i < items_count; i++) {
        list_item* p = items[i];

        if(p->value == value) {
            // Remove the slot
            list_item_free(p);

            // If at the end, skip block moving
            if(i != items_count - 1) {
                // Move block
                void* from = &items[i + 1];
                void* to = &items[i];
                const size_t blocks = items_count - 1 - i;

                // Move array pointers along
                memmove(to, from, sizeof(list_item*) * blocks);
            }

            l->length--;
            return true;
        }
    }

    return false;
}

void list_dispose_item_by_value(list* l, void* value) {
    assert(l);
    assert(value);
    assert(l->length > 0);

    list_item** items = list_enumerable(l);
    const size_t items_count = list_count(l);

    for(unsigned int i = 0; i < items_count; i++) {
        list_item* p = items[i];

        if(p->value == value) {
            if(p->value) {
                xmemory_free(p->value);
            }

            // Remove the slot
            list_item_free(p);

            // If at the end, skip block moving
            if(i != items_count - 1) {
                // Move block
                void* from = &items[i + 1];
                void* to = &items[i];
                const size_t blocks = items_count - 1 - i;

                // Move array pointers along
                memmove(to, from, sizeof(list_item*) * blocks);
            }

            l->length--;
            break;
        }
    }
}

void list_remove_item_at(list* l, unsigned int index) {
    assert(l);
    assert(l->length > 0);
    assert(index < l->length);

    list_item** items = list_enumerable(l);
    const size_t items_count = list_count(l);

    list_item* p = items[index];

    // Remove the slot
    list_item_free(p);

    // If at the end, skip block moving
    if(index != items_count - 1) {
        // Move block
        void* from = &items[index + 1];
        void* to = &items[index];
        const size_t blocks = items_count - 1 - index;

        // Move array pointers along
        memmove(to, from, sizeof(list_item*) * blocks);
    }

    l->length--;
}

void list_dispose_item_at(list* l, unsigned int index) {
    assert(l);
    assert(l->length > 0);
    assert(index < l->length);

    list_item** items = list_enumerable(l);
    const size_t items_count = list_count(l);

    list_item* p = items[index];

    if(p->value) {
        xmemory_free(p->value);
    }

    // Remove the slot
    list_item_free(p);

    // If at the end, skip block moving
    if(index != items_count - 1) {
        // Move block
        void* from = &items[index + 1];
        void* to = &items[index];
        const size_t blocks = items_count - 1 - index;

        // Move array pointers along
        memmove(to, from, sizeof(list_item*) * blocks);
    }

    l->length--;
}

list_item* list_get_item(const list* l, const unsigned int index) {
    assert(l);
    assert(index < l->length);

    return l->data[index];
}

void* list_get_value(const list* l, const unsigned index) {
    list_item* item = list_get_item(l, index);

    if(item == NULL) {
        return NULL;
    }

    return item->value;
}

unsigned int list_count(const list* l) {
    assert(l);

    return l->length;
}

void list_clear(list* l) {
    assert(l);

    for(int i = 0; i < l->length; i++) {
        list_item* item = (list_item*)l->data[i];

        list_item_free(item);

        //l->data[i] = NULL;
        // xmemory_free(item->value); // NOTE: This should be done by the caller
    }

    l->length = 0;
}

void list_free(list* l) {
    assert(l);
    list_clear(l);

    if(l->alloc_filename) xmemory_free(l->alloc_filename);

    xmemory_free(l->data);
    xmemory_free(l);
}

void list_take(const list* source, const size_t count, list* target) {
    list_item** items = list_enumerable(source);
    const size_t items_count = count <= list_count(source) ? count : list_count(source);

    for(unsigned int i = 0; i < items_count; i++) {
        list_item* item = items[i];
        list_add(target, item->value);
    }
}

void list_take_right(const list* source, const size_t count, list* target) {
    list_item** items = list_enumerable(source);
    const size_t total_items_count = list_count(source);
    const size_t items_count = count <= list_count(source) ? count : list_count(source);

    for(unsigned int i = 0; i < items_count; i++) {
        list_item* item = items[total_items_count - i - 1];
        list_add(target, item->value);
    }
}
