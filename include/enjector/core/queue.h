/*
* queue.h
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

struct queue_item_t;

typedef struct queue_item_t {
    void* data;

    struct queue_item_t* next;
} queue_item;

typedef struct queue_t {
    queue_item* root;
    unsigned int size;
} queue;

queue*		 queue_create();
void		 queue_free(queue* queue);

void		 queue_clear(queue* q);

unsigned int queue_count(queue* queue);
void		 queue_push(queue* queue, void* data);
void*		 queue_pop(queue* queue);
