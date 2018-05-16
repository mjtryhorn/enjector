/*
* queue.c
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

#include <enjector/core/queue.h>
#include <enjector/core/xmemory.h>

#include <assert.h>
#include <stdlib.h>

queue* queue_create() {
    queue* q = xmemory_new(queue);
    q->root = NULL;
    q->size = 0;

    return q;
}

void queue_free(queue* queue) {
    assert(queue);

    queue_clear(queue);

    xmemory_free(queue);
}

void queue_clear(queue* queue) {
    assert(queue);

    while(queue->size-- > 0) {
        queue_pop(queue);
    }
}

unsigned int queue_count(queue* queue) {
    assert(queue);

    return queue->size;
}

void queue_push(queue* queue, void* data) {
    assert(queue);

    queue_item* item = xmemory_new(queue_item);
    item->data = data;

    item->next = queue->root;
    queue->root = item;

    queue->size++;
}

void* queue_pop(queue* queue) {
    void* data = NULL;

    if (queue->size > 0) {
        queue_item* item = queue->root;
        data = item->data;

        queue->root = item->next;
        queue->size--;

        xmemory_free(item);
    }

    return data;
}
