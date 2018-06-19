/*
* json.c
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

#include <enjector/core/list.h>
#include <enjector/core/map.h>
#include <enjector/core/string_buffer.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <stdlib.h>

void json_serialise_string(string_buffer* stream, const char* name, char* value);
void json_serialise_char_array_fixed(string_buffer* stream, const char* name, char* value, int size);
void json_serialise_integer(string_buffer* stream, char* name, int value);
//void json_serialise_long(string_buffer* stream, char* name, long value);
void json_serialise_float(string_buffer* stream, char* name, float value);
void json_serialise_object(string_buffer* stream, char* name, char* json);
void json_serialise_bool(string_buffer* stream, char* name, bool value);

void json_serialise_comma(string_buffer* json);;
//void json_serialise_string(string_buffer* json, const char* name, const char* value);
void json_serialise_int(string_buffer* json, const char* name, int value);
void json_serialise_short(string_buffer* json, const char* name, short value);
void json_serialise_long(string_buffer* json, const char* name, long value);
void json_serialise_unsigned_long(string_buffer* json, const char* name, size_t value);
void json_serialise_char(string_buffer* json, const char* name, char value);
void json_serialise_unsigned_char(string_buffer* json, const char* name, char value);
void json_serialise_object_start(string_buffer* json, char* name);
void json_serialise_object_end(string_buffer* json);
void json_serialise_array_start(string_buffer* json, char* name);
void json_serialise_array_end(string_buffer* json);

bool json_deserialise_object(char* s, map* deserialised_attrs);
bool json_deserialise_array(char* s, list* deserialised_list);
bool json_deserialise_map(char* s, map* deserialised_map);
bool json_deserialise_string(map* attrs, const char* name, char** value);
bool json_deserialise_char_array(map* attrs, const char* name, char* value, size_t size);
bool json_deserialise_integer(map* attrs, const char* name, int* value);
bool json_deserialise_long(map* attrs, const char* name, long* value);
bool json_deserialise_float(map* attrs, const char* name, float* value);
bool json_deserialise_bool(map* attrs, const char* name, bool* value);

#define SERIALISABLE_begin(cls) string_buffer_append(stream, "{"); {
#define SERIALISABLE_end() string_buffer_append(stream, "}"); }

typedef enum JSONMode_t {
    JSON_SERIALISE,
    JSON_DESERIALISE,
    JSON_FREE
} JSONMode;

#define DEFINE_SERIALISE(cls, f) \
string_buffer* cls ## _serialise(cls* instance) {\
	string_buffer* stream = string_buffer_create_empty();\
	SERIALISABLE_begin(cls);\
	map* attrs = NULL;\
	JSONMode mode_serialise = JSON_SERIALISE;\
	size_t _p = 0;\
	f; SERIALISABLE_end();\
	return stream;\
}

#define DEFINE_DESERIALISE(cls, f) \
cls* cls ## _deserialise(char* json) {\
	cls* instance = NULL;\
	string_buffer* stream = NULL;\
	map* attrs = map_create();\
	if (json_deserialise_object(json, attrs)) {\
		instance = xmemory_new(cls);\
		bzero(instance, sizeof(cls));\
		JSONMode mode_serialise = JSON_DESERIALISE;\
		int _p = 0;\
		f;\
	}\
	map_item** attr_items = map_enumerable(attrs);\
	for (size_t i = 0; i < map_count(attrs); i++) xmemory_free(attr_items[i]->value);\
	map_free(attrs);\
	return instance;\
}

#define DEFINE_CREATE(cls) \
cls* cls ## _create() {\
	return xmemory_new(cls);\
}

#define DEFINE_FREE(cls, f) \
void cls ## _dispose(cls* instance) {\
	string_buffer* stream = NULL;\
	map* attrs = NULL;\
	JSONMode mode_serialise = JSON_FREE;\
	int _p = 0;\
	f;\
	xmemory_free(instance);\
}

#define serialisable(cls, f)\
	DEFINE_SERIALISE(cls, f)\
	DEFINE_DESERIALISE(cls, f)\
	DEFINE_CREATE(cls)\
	DEFINE_FREE(cls, f)

#define extern_serialisable(cls)\
extern string_buffer* cls ## _serialise(cls* instance);\
extern cls* cls ## _deserialise(char* json);\
extern void cls ## _dispose(cls* instance);

#define as_string_save(stream, instance, prop) json_serialise_string(stream, #prop, instance->prop);

#define as_char_array_save(stream, instance, prop, size) json_serialise_char_array_fixed(stream, #prop, instance->prop, size);

#define as_bool_save(stream, instance, prop) json_serialise_bool(stream, #prop, instance->prop);

#define as_integer_save(stream, instance, prop) json_serialise_integer(stream, #prop, instance->prop);

#define as_long_save(stream, instance, prop) json_serialise_long(stream, #prop, instance->prop);

#define as_float_save(stream, instance, prop) json_serialise_float(stream, #prop, instance->prop);

#define as_list_save(stream, type, instance, prop) \
string_buffer_append(stream, "\"");\
string_buffer_append(stream, #prop);\
if (instance->prop == NULL) {\
	string_buffer_append(stream, "\":null");\
} else {\
	string_buffer_append(stream, "\":[");\
	for (size_t i = 0; i < list_count(instance->prop); i++) {\
		if (i > 0) { string_buffer_append(stream, ","); }\
		list_item* item = list_get_item(instance->prop, i);\
		string_buffer* json = type ## _serialise(item->value);\
		string_buffer_append(stream, json->data);\
		string_buffer_free(json);\
	}\
	string_buffer_append(stream, "]");\
}

#define as_map_save(stream, type, instance, prop) \
	string_buffer_append(stream, "\"");\
	string_buffer_append(stream, #prop);\
	if (instance->prop == NULL) {\
		string_buffer_append(stream, "\":null");\
	} else {\
		string_buffer_append(stream, "\":{");\
		map_item** items = map_enumerable(instance->prop);\
		for (size_t i = 0; i < map_count(instance->prop); i++) {\
			if (i > 0) { string_buffer_append(stream, ","); }\
			map_item* item = items[i];\
			string_buffer_append(stream, "\"");\
			string_buffer_append(stream, item->name);\
			string_buffer_append(stream, "\":");\
			string_buffer* json = type ## _serialise(item->value);\
			string_buffer_append(stream, json->data);\
			string_buffer_free(json);\
		}\
		string_buffer_append(stream, "}");\
}

#define as_strings_save(stream, instance, prop) \
string_buffer_append(stream, "\"");\
string_buffer_append(stream, #prop);\
if (instance->prop == NULL) {\
	string_buffer_append(stream, "\":null");\
} else {\
	string_buffer_append(stream, "\":[");\
	for (size_t i = 0; i < list_count(instance->prop); i++) {\
		if (i > 0) { string_buffer_append(stream, ","); }\
		list_item* item = list_get_item(instance->prop, i);\
		string_buffer_append(stream, "\"");\
		string_buffer_append(stream, item->value);\
		string_buffer_append(stream, "\"");\
	}\
	string_buffer_append(stream, "]");\
}

#define as_integers_save(stream, instance, prop) \
string_buffer_append(stream, "\"");\
string_buffer_append(stream, #prop);\
if (instance->prop == NULL) {\
	string_buffer_append(stream, "\":null");\
} else {\
	string_buffer_append(stream, "\":[");\
	for (size_t i = 0; i < list_count(instance->prop); i++) {\
		if (i > 0) { string_buffer_append(stream, ","); }\
		list_item* item = list_get_item(instance->prop, i);\
		string_buffer_append_format(stream, "%d", *((int*) item->value));\
	}\
	string_buffer_append(stream, "]");\
}

#define as_object_save(stream, type, instance, prop) \
string_buffer* json = type ## _serialise(instance->prop);\
json_serialise_object(stream, #prop, json->data);\
string_buffer_free(json);

#define as_string_load(attrs, instance, prop) json_deserialise_string(attrs, #prop, &instance->prop);

#define as_char_array_load(attrs, instance, prop, size) json_deserialise_char_array(attrs, #prop, instance->prop, size);

#define as_integer_load(attrs, instance, prop) json_deserialise_integer(attrs, #prop, &instance->prop);

#define as_long_load(attrs, instance, prop) json_deserialise_long(attrs, #prop, &instance->prop);

#define as_float_load(attrs, instance, prop) json_deserialise_float(attrs, #prop, &instance->prop);

#define as_bool_load(attrs, instance, prop) json_deserialise_bool(attrs, #prop, &instance->prop);

#define as_object_load(attrs, type, instance, prop) {\
	map_item* m = map_get_item(attrs, #prop);\
	if (m != NULL) {\
		char* json = m->value;\
		instance->prop = type ## _deserialise(json);\
	}\
}

#define as_list_load(attrs, type, instance, prop) {\
	map_item* m = map_get_item(attrs, #prop);\
	if (m != NULL) {\
		char* json = m->value;\
		list* array_items = list_create();\
		if (json_deserialise_array(json, array_items)) {\
			instance->prop = array_items;\
			size_t count = list_count(array_items);\
			size_t i = 0;\
			while(i < count) {\
				list_item* array_item = list_get_item(array_items, i);\
				char* array_item_json = array_item->value;\
				type* item_instance = type ## _deserialise(array_item_json);\
				array_item->value = (void*) item_instance;\
				xmemory_free(array_item_json);\
				i++;\
			}\
		}\
	}\
}

#define as_map_load(attrs, type, instance, prop) {\
	map_item* m = map_get_item(attrs, #prop);\
	if (m != NULL) {\
		char* json = m->value;\
		map* map_items = map_create();\
		if (json_deserialise_map(json, map_items)) {\
			instance->prop = map_items;\
			size_t count = map_count(map_items);\
			size_t i = 0;\
			while(i < count) {\
				map_item* map_item = map_get_item_at(map_items, i);\
				char* map_item_name = map_item->name;\
				char* map_item_json = map_item->value;\
				type* item_instance = type ## _deserialise(map_item_json);\
				map_item->value = (void*) item_instance;\
				xmemory_free(map_item_json);\
				i++;\
			}\
		}\
	}\
}

#define as_strings_load(attrs, instance, prop) {\
	map_item* m = map_get_item(attrs, #prop);\
	if (m != NULL) {\
		char* json = m->value;\
		list* array_items = list_create();\
		if (json_deserialise_array(json, array_items)) {\
			instance->prop = list_create();\
			size_t array_count = list_count(array_items);\
			size_t i = 0;\
			while(i < array_count) {\
				list_item* array_item = list_get_item(array_items, i);\
				char* item_instance = array_item->value;\
				list_add(instance->prop, item_instance);\
				i++;\
			}\
		}\
		list_free(array_items);\
	}\
}

#define as_integers_load(attrs, instance, prop) {\
	map_item* m = map_get_item(attrs, #prop);\
	if (m != NULL) {\
		char* json = m->value;\
		list* array_items = list_create();\
		if (json_deserialise_array(json, array_items)) {\
			instance->prop = list_create();\
			size_t array_count = list_count(array_items);\
			size_t i = 0;\
			while(i < array_count) {\
				list_item* array_item = list_get_item(array_items, i);\
				char* item_instance = array_item->value;\
				int item_value = atoi(item_instance);\
				void* item_data = xmemory_malloc(sizeof(int));\
				memcpy(item_data, &item_value, sizeof(int));\
				list_add(instance->prop, item_data);\
				text_free(item_instance);\
				i++;\
			}\
		}\
		list_free(array_items);\
	}\
}

#define as_string_dispose(instance, prop) {\
	if (instance->prop) {\
		text_free(instance->prop);\
		instance->prop = NULL;\
	}\
}

#define as_object_dispose(type, instance, prop) {\
	if (instance->prop) {\
		type ## _dispose(instance->prop);\
		instance->prop = NULL;\
	}\
}

#define as_list_dispose(type, instance, prop) {\
	if (instance->prop) {\
		list_dispose_items_of(instance->prop, type);\
		instance->prop = NULL;\
	}\
}

#define as_map_dispose(type, instance, prop) {\
	if (instance->prop) {\
		map_item_disposes_of(instance->prop, type);\
	}\
}

#define as_strings_dispose(instance, prop) {\
	if (instance->prop) {\
		list* array_items = instance->prop;\
		size_t array_count = list_count(array_items);\
		size_t i = 0;\
		while(i < array_count) {\
			list_item* array_item = list_get_item(array_items, i);\
			xmemory_free(array_item->value);\
			i++;\
		}\
		list_free(array_items);\
		instance->prop = NULL;\
	}\
}

#define as_integers_dispose(instance, prop) {\
	if (instance->prop) {\
		list* array_items = instance->prop;\
		size_t array_count = list_count(array_items);\
		size_t i = 0;\
		while(i < array_count) {\
			list_item* array_item = list_get_item(array_items, i);\
			xmemory_free(array_item->value);\
			i++;\
		}\
		list_free(array_items);\
		instance->prop = NULL;\
	}\
}

#define as_save_delim_opt if (_p++ > 0) { string_buffer_append(stream, ","); }

#define as_string(prop) if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_string_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_string_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) { as_string_dispose(instance, prop); }

#define as_char_array(prop) if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_char_array_save(stream, instance, prop, sizeof(instance->prop)); } else if (mode_serialise == JSON_DESERIALISE) { as_char_array_load(attrs, instance, prop, sizeof(instance->prop)); } else if (mode_serialise == JSON_FREE) { }

#define as_bool(prop)  if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_bool_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_bool_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) {  }

#define as_integer(prop)  if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_integer_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_integer_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) {  }

#define as_long(prop)  if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_long_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_long_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) {  }

#define as_float(prop)  if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_float_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_float_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) {  }

#define as_object(type, prop)if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_object_save(stream, type, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_object_load(attrs, type, instance, prop); } else if (mode_serialise == JSON_FREE) { as_object_dispose(type, instance, prop); }

#define as_list(type, prop) if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_list_save(stream, type, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_list_load(attrs, type, instance, prop); } else if (mode_serialise == JSON_FREE) { as_list_dispose(type, instance, prop); }

#define as_map(type, prop) if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_map_save(stream, type, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_map_load(attrs, type, instance, prop); } else if (mode_serialise == JSON_FREE) { as_map_dispose(type, instance, prop); }

#define as_strings(prop) if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_strings_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_strings_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) { as_strings_dispose(instance, prop); }

#define as_integers(prop) if (mode_serialise == JSON_SERIALISE) { as_save_delim_opt; as_integers_save(stream, instance, prop); } else if (mode_serialise == JSON_DESERIALISE) { as_integers_load(attrs, instance, prop); } else if (mode_serialise == JSON_FREE) { as_integers_dispose(instance, prop); }
