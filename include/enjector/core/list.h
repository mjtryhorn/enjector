/*
* list.h
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

#include <enjector/core/xmemory.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct list_item_t {
    void* value;
    char* type;
} list_item;

typedef struct list_t {
    list_item** data;
    unsigned int length;
    unsigned int stream_capacity;

    char* alloc_filename;
    unsigned int alloc_line;
} list;

list*			_list_create(const char* filename, unsigned int line);
#define			list_create() _list_create(__FILE__, __LINE__);
void			list_add(list* l, void* value);
void			list_add_with_type(list* l, const char* type, void* value);
list_item*		list_get_item(const list* l, const unsigned int index);	// TODO: list_get_item_at
void*			list_get_value(const list* l, const unsigned index);
list_item**		list_enumerable(const list* l);
void			list_remove_item(list* l, list_item* item);
void			list_remove_item_at(list* l, unsigned int index);
void			list_dispose_item_at(list* l, unsigned int index);
void			list_dispose_item_by_value(list* l, void* value);
bool			list_remove_item_by_value(list* l, void* value);
unsigned int	list_count(const list* l);
void			list_clear(list* l);
void			list_free(list* l);
void			list_take_right(const list* source, const size_t count, list* target);
void			list_take(const list* source, const size_t count, list* target);

#define list(type, name) \
    list* name = list_create();\
    typedef type name ## _type;

#define list_of(type, name) \
    list* name = NULL;\
    typedef type name ## _type;

#define list_define(type, name) \
    typedef type name ## _type;

#define list_cast(type, name, source) \
    typedef type name ## _type;\
    list* name = source;

#define list_foreach_begin(source, value_alias) {\
    list_item** value_alias ## items = list_enumerable(source);\
    size_t value_alias ## items_count = list_count(source);\
    for (unsigned int value_alias ## _i = 0; value_alias ## _i < value_alias ## items_count; value_alias ## _i++) {\
    list_item* value_alias ## item = value_alias ## items[value_alias ## _i];\
    source ## _type* value_alias = (source ## _type*) value_alias ## item->value;

#define list_foreach_end }}

#define list_foreach_of_begin(source, type, value_alias) {\
    list_item** value_alias ## items = list_enumerable(source);\
    size_t value_alias ## items_count = list_count(source);\
    for (unsigned int value_alias ## _i = 0; value_alias ## _i < value_alias ## items_count; value_alias ## _i++) {\
    list_item* value_alias ## item = value_alias ## items[value_alias ## _i];\
    type value_alias = (type) value_alias ## item->value;

#define list_foreach_of_end }}

#define list_filter(source, value_alias, target, condition)\
    list_foreach_begin(source, value_alias) { if (condition) list_add(target, value_alias); } list_foreach_of_end

#define list_get(source, index) ((source ## _type*) list_get_item(source, index)->value)

#define list_map_begin(source, source_value_alias, target, target_value_alias) \
    list_foreach_begin(source, source_value_alias) {\
    target ## _type* target_value_alias = xmemory_new(target ## _type);\
    list_add(target, target_value_alias);

#define list_map_end } list_foreach_of_end

#define list_dispose_items(source) {\
	list_foreach_of_begin(source, void*, source_value_alias) {\
		xmemory_free(source_value_alias);\
	} list_foreach_of_end\
	list_clear(source);\
}

#define list_dispose(source) {\
	list_dispose_items(source);\
	list_free(source);\
	source = NULL;\
}

#define list_dispose_items_of(source, type) {\
	list_foreach_of_begin(source, type*, source_value_alias) {\
		type ## _dispose(source_value_alias);\
	} list_foreach_of_end\
	list_free(source);\
	source = NULL;\
}

#define list_add_new(source, value_alias, f) {\
    source ## _type* value_alias = xmemory_new(source ## _type); \
    list_add(source, value_alias);\
    f\
}

#define list_first(source, value_alias) \
    source ## _type* value_alias = list_count(source) == 0 ? NULL : (source ## _type*) list_get_item(source, 0)->value;

#define list_first_of(source, type) \
    list_count(source) == 0 ? NULL : (type*) list_get_item(source, 0)->value;

#define list_get_of(source, index, type) \
    list_count(source) == 0 ? NULL : (type*) list_get_item(source, index)->value;

#define list_last(source, value_alias) \
    source ## _type* value_alias = list_count(source) == 0 ? NULL : (source ## _type*) list_get_item(source, list_count(source) - 1)->value;

#define list_last_of(source, type) \
    list_count(source) == 0 ? NULL : (type*) list_get_item(source, list_count(source) - 1)->value;
