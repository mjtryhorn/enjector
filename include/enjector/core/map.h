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

#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct map_item_t {
    char* type;
    char* name;
    void* value;
} map_item;

typedef struct map_t {
    map_item** data;
    size_t length;
    unsigned int stream_capacity;

    char* alloc_filename;
    int alloc_line;
} map;

map*		 _map_create(const char* filename, unsigned int line);
#define		 map_create() _map_create(__FILE__, __LINE__);
bool 	     map_set(map* m, const char* name, void* value);
bool 	     map_set_with_type(map* m, const char* name, const char* type, void* value);
map_item*	 map_get_item(const map* m, const char* name);
bool 	     map_exists(const map* m, const char* name);
void*		 map_get_value(const map* m, const char* name);
void		 map_clear(map* m);
void		 map_remove(map* m, const char* name);
void		 map_item_dispose(map* m, const char* name);
map_item**	 map_enumerable(const map* m);
unsigned int map_count(const map* m);
void		 map_free(map* m);
void		 map_item_free(map_item* item);
map_item*	 map_get_item_at(const map* m, unsigned int index);

#define map_of(type, name) \
    map* name = NULL;\
    typedef type name ## _type;

#define map(type, name) \
    map* name = map_create();\
    typedef type name ## _type;

#define map_foreach_begin(source, key_alias, value_alias)\
    map_item** items = map_enumerable(source);\
    size_t items_count = map_count(source);\
    for (unsigned int i = 0; i < items_count; i++) {\
	    map_item* item = items[i];\
	    const char* key_alias = item->name;\
	    source ## _type* value_alias = (source ## _type*) item->value;

#define map_foreach_end }

#define map_foreach_of_begin(source, key_alias, value_type, value_alias) {\
    map_item** items = map_enumerable(source);\
    size_t items_count = map_count(source);\
    for (unsigned int _i = 0; _i < items_count; _i++) {\
	    map_item* item = items[_i];\
	    const char* key_alias = item->name;\
	    value_type value_alias = (value_type) item->value;

#define map_foreach_of_end }}

#define map_filter(source, key_alias, value_alias, target, condition)\
    map_foreach_begin(source, key_alias, value_alias) { if (condition) map_set(target, key, value_alias); } map_foreach_end

#define map_set_new(source, name, value_alias, f) {\
    source ## _type* value_alias = xmemory_new(source ## _type); \
    map_set(source, name, value_alias);\
    f\
}

#define map_get(source, name) (source ## _type*) map_get_value(source, name)

// TODO: what about typed ones like JSON
#define map_dispose(source) {\
	map_foreach_of_begin(source, source_key_alias, void*, source_value_alias) {\
		xmemory_free(source_value_alias);\
	} map_foreach_of_end;\
	map_free(source);\
	source = NULL;\
}

#define map_clear_dispose(source) {\
	map_foreach_of_begin(source, source_key_alias, void*, source_value_alias) {\
		xmemory_free(source_value_alias);\
	} map_foreach_of_end;\
	map_clear(source);\
}

#define map_item_disposes_of(source, type) {\
	map_item** items = map_enumerable(source);\
	size_t items_count = map_count(source);\
	for (unsigned int i = 0; i < items_count; i++) {\
		map_item* item = items[i];\
		const char* key_alias = item->name;\
		type ## _dispose(item->value);\
	}\
	map_free(source);\
	source = NULL;\
}

// TODO: re-examine: can this be derived from map_of?
#define map_item_dispose_of(source, name, type) {\
	void* p = map_get_value(source, name);\
	if (p) type ## _dispose(p);\
	map_remove(source, name);\
}
