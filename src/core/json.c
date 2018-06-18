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

#include <enjector/core/json.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//#define DEBUG_JSON_STATE
//#define DEBUG_JSON

void json_serialise_string(string_buffer* stream, const char* name, char* value) {
    string_buffer_append(stream, "\"");
    string_buffer_append(stream, name);

    if (value) {
        string_buffer_append(stream, "\":\"");

        // Encode quotes
        if (strstr(value, "\"")) {
            char* value_encoded = text_replace(value, "\"", "\\\"");
            string_buffer_append(stream, value_encoded);
            xmemory_free(value_encoded);
        } else {
            string_buffer_append(stream, value);
        }

        string_buffer_append(stream, "\"");
    } else {
        string_buffer_append(stream, "\":null");
    }
}

void json_serialise_char_array_fixed(string_buffer* stream, const char* name, char* value, int size) {
    json_serialise_string(stream, name, value);
}

void json_serialise_integer(string_buffer* stream, char* name, int value) {
    string_buffer_append(stream, "\"");
    string_buffer_append(stream, name);
    string_buffer_append(stream, "\":");

    char buf[20];
    snprintf(buf, 20, "%d", value);
    string_buffer_append(stream, buf);
}

//void json_serialise_long(string_buffer* stream, char* name, long value) {
//	string_buffer_append(stream, "\"");
//	string_buffer_append(stream, name);
//	string_buffer_append(stream, "\":");
//
//	char buf[50];
//	sprintf(buf, "%ld", value);
//	string_buffer_append(stream, buf);
//}

void json_serialise_float(string_buffer* stream, char* name, float value) {
    string_buffer_append(stream, "\"");
    string_buffer_append(stream, name);
    string_buffer_append(stream, "\":");

    char buf[50];
    snprintf(buf, 50, "%f", value);
    string_buffer_append(stream, buf);
}

void json_serialise_bool(string_buffer* stream, char* name, bool value) {
    string_buffer_append(stream, "\"");
    string_buffer_append(stream, name);
    string_buffer_append(stream, "\":");
    string_buffer_append(stream, value ? "true" : "false");
}

void json_serialise_object(string_buffer* stream, char* name, char* json) {
    string_buffer_append(stream, "\"");
    string_buffer_append(stream, name);
    string_buffer_append(stream, "\":");
    string_buffer_append(stream, json);
}

void json_serialise_comma(string_buffer* json) {
    string_buffer_append(json, ",");
}

//void json_serialise_string(string_buffer* json, const char* name, const char* value)
//{
//	if (value)
//	{
//		string_buffer_append_format(json, "\"%s\":\"%s\"", name, value);
//	}
//	else
//	{
//		string_buffer_append_format(json, "\"%s\":null", name);
//	}
//}

void json_serialise_int(string_buffer* json, const char* name, int value) {
    string_buffer_append_format(json, "\"%s\":%d", name, value);
}

void json_serialise_short(string_buffer* json, const char* name, short value) {
    string_buffer_append_format(json, "\"%s\":%h", name, value);
}

void json_serialise_long(string_buffer* json, const char* name, long value) {
    string_buffer_append_format(json, "\"%s\":%ld", name, value);
}

void json_serialise_unsigned_long(string_buffer* json, const char* name, size_t value) {
    string_buffer_append_format(json, "\"%s\":%lu", name, value);
}

void json_serialise_char(string_buffer* json, const char* name, char value) {
    string_buffer_append_format(json, "\"%s\":\"%c\"", name, value);
}

void json_serialise_unsigned_char(string_buffer* json, const char* name, char value) {
    string_buffer_append_format(json, "\"%s\":\"%u\"", name, value);
}

void json_serialise_object_start(string_buffer* json, char* name) {
    if (name == NULL) {
        string_buffer_append(json, "{");
    } else {
        string_buffer_append_format(json, "\"%s\":{", name);
    }
}

void json_serialise_object_end(string_buffer* json) {
    string_buffer_append(json, "}");
}

void json_serialise_array_start(string_buffer* json, char* name) {
    string_buffer_append_format(json, "\"%s\":[", name);
}

void json_serialise_array_end(string_buffer* json) {
    string_buffer_append(json, "]");
}

bool json_deserialise_string(map* attrs, const char* name, char** value) {
    map_item* item = map_get_item(attrs, name);

    if (item != NULL) {

        const char* value_encoded = (char*)item->value;

        // Decode quotes
        if (strstr(value_encoded, "\\\"")) {
            *value = text_replace(value_encoded, "\\\"", "\"");
        }
        // TODO: doesn't support both
        else if (strstr(value_encoded, "\\\'")) {
            *value = text_replace(value_encoded, "\\\'", "\'");
        } else {
            *value = text_clone(value_encoded);
        }
    }

    return true;
}

bool json_deserialise_char_array(map* attrs, const char* name, char* value, int size) {
    map_item* item = map_get_item(attrs, name);

    if (item != NULL) {

        const char* value_encoded = (const char*)item->value;
        char* value_result = (char*) value_encoded;
        bool value_result_dispose = false;

        // Decode quotes
        if (strstr(value_encoded, "\\\"")) {
            value_result = text_replace(value_encoded, "\\\"", "\"");
            value_result_dispose = true;
        }
        // TODO: doesn't support both
        else if (strstr(value_encoded, "\\\'")) {
            value_result = text_replace(value_encoded, "\\\'", "\'");
            value_result_dispose = true;
        }

        size_t value_result_length = text_length(value_result);

        if (value_result_length> size) {
            value_result_length = size - 1;
        }

        text_copy_length(value, value_result, value_result_length);
        value[value_result_length] = 0;

        if (value_result_dispose) {
            text_free(value_result);
        }
    }

    return true;
}

bool json_deserialise_integer(map* attrs, const char* name, int* value) {
    map_item* item = map_get_item(attrs, name);

    if (item == NULL) {
        *value = 0;
    } else {
        *value = atoi(item->value);
    }

    return true;
}

bool json_deserialise_long(map* attrs, const char* name, long* value) {
    map_item* item = map_get_item(attrs, name);

    if (item == NULL) {
        *value = 0;
    } else {
        *value = atol(item->value);
    }

    return true;
}

bool json_deserialise_float(map* attrs, const char* name, float* value) {
    map_item* item = map_get_item(attrs, name);

    if (item == NULL) {
        *value = 0;
    } else {
        *value = (float) atof(item->value);
    }

    return true;
}

bool json_deserialise_bool(map* attrs, const char* name, bool* value) {
    map_item* item = map_get_item(attrs, name);

    if (item == NULL) {
        *value = false;
    } else {
        *value = text_compare_nocase(item->value, "true") == 1;
    }

    return true;
}

#pragma region Parser

typedef enum DeserialiserState_t {
    SERIALISABLE_STATE_ERROR = -1,
    SERIALISABLE_STATE_INIT = 0,
    SERIALISABLE_STATE_OBJECT_START = 1,
    SERIALISABLE_STATE_OBJECT_END = 2,
    SERIALISABLE_STATE_ARRAY_START = 3,
    SERIALISABLE_STATE_ARRAY_END = 4,
    SERIALISABLE_STATE_STRING_START = 5,
    SERIALISABLE_STATE_STRING_END = 6,
    SERIALISABLE_STATE_NAME_START = 7,
    SERIALISABLE_STATE_NAME_END = 8,
    SERIALISABLE_STATE_SEPARATOR = 9,
    SERIALISABLE_STATE_COMMA = 10,
    SERIALISABLE_STATE_VALUE_START = 11,
    SERIALISABLE_STATE_VALUE_END = 12
} DeserialiserState;

typedef enum DeserialiserValueType_t {
    SERIALISABLE_VALUE_TYPE_UNKNOWN = -1,
    SERIALISABLE_VALUE_TYPE_STRING = 0,
    SERIALISABLE_VALUE_TYPE_NUM = 1,
    SERIALISABLE_VALUE_TYPE_ARRAY = 2,
    SERIALISABLE_VALUE_TYPE_OBJECT = 3,
    SERIALISABLE_VALUE_TYPE_NULL = 4,
    SERIALISABLE_VALUE_TYPE_BOOL_FALSE = 5,
    SERIALISABLE_VALUE_TYPE_BOOL_TRUE = 6
} DeserialiserValueType;

bool isValidNameCharStart(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

bool isValidNameChar(char c) {
    return isValidNameCharStart(c) || c == '-';
}

bool isEmptySpaceChar(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isNumberChar(char c) {
    return (c >= '0' && c <= '9') || c == '-' || c == '.';
}

bool isValueSymbol(char c) {
    return (c >= 33 && c <= 47) && (c >= 58 && c <= 64) && (c >= 91 && c <= 96) && (c >= 123 && c <= 126) &&
           c != 34 && c != 39;
}

char* substring(const char* buffer, size_t pos_start, size_t pos_end) {
    size_t len = pos_end - pos_start;

    if (len == 0) {
        return NULL;
    }

    char* temp = (char*)xmemory_malloc(len + 1);
//	bzero(temp, len + 1);
    memcpy(temp, buffer + pos_start, len);
    temp[len] = 0;
    return temp;
}

bool json_deserialise_object(char* s, map* deserialised_attrs) {

    DeserialiserState state = SERIALISABLE_STATE_INIT;
    DeserialiserState previous_state = SERIALISABLE_STATE_INIT;

    DeserialiserValueType value_type = SERIALISABLE_VALUE_TYPE_UNKNOWN;
    bool in_string = false;		// Are we in a string
    char in_string_char = 0;	// How did the string start?

    const char* buffer = s;
    size_t length = strlen(s);
    size_t index = 0;
    size_t pos_start = 0;
    size_t pos_end = 0;
    size_t depth_array = 0;
    size_t depth_object = 0;

    char* name = NULL;
    char* value = NULL;

    while (index < length) {
        char c = buffer[index];

        if (c == '\\') {// && state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) {
            state = SERIALISABLE_STATE_VALUE_START;
            index += 2;
            //            c = buffer[index];
            continue;
        } else {
            if (c == 'n' && state == SERIALISABLE_STATE_SEPARATOR && !strncmp(&buffer[index], "null", 4)) {
                state = SERIALISABLE_STATE_VALUE_START;
                value_type = SERIALISABLE_VALUE_TYPE_NULL;
            } else if (c == 't' && state == SERIALISABLE_STATE_SEPARATOR && !strncmp(&buffer[index], "true", 4)) {
                state = SERIALISABLE_STATE_VALUE_START;
                value_type = SERIALISABLE_VALUE_TYPE_BOOL_TRUE;
            } else if (c == 'f' && state == SERIALISABLE_STATE_SEPARATOR && !strncmp(&buffer[index], "false", 4)) {
                state = SERIALISABLE_STATE_VALUE_START;
                value_type = SERIALISABLE_VALUE_TYPE_BOOL_FALSE;
            } else {
                switch (c) {
                case '{':

                    // Ignore JSON symbols inside strings
                    if (in_string) {
                        index++;
                        continue;
                    }

                    depth_object++;

                    //if (depth_object > 1) { index++; continue; }*/

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in string value
                    else if (state == SERIALISABLE_STATE_SEPARATOR) {
                        state = SERIALISABLE_STATE_VALUE_START;
                        value_type = SERIALISABLE_VALUE_TYPE_OBJECT;
                    } else if (state == SERIALISABLE_STATE_INIT) state = SERIALISABLE_STATE_OBJECT_START;
                    else if (state == SERIALISABLE_STATE_ARRAY_START) state = SERIALISABLE_STATE_OBJECT_START;
                    else if (state == SERIALISABLE_STATE_COMMA) state = SERIALISABLE_STATE_OBJECT_START;
                    else state = SERIALISABLE_STATE_ERROR;

                    break;

                case '}':

                    // Ignore JSON symbols inside strings
                    if (in_string) {
                        index++;
                        continue;
                    }

                    depth_object--;

                    if (depth_object > 1) {
                        index++;
                        continue;
                    }

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_END; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in string value
                    else if (state == SERIALISABLE_STATE_VALUE_END) state = SERIALISABLE_STATE_OBJECT_END;
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NUM) {
                        state = SERIALISABLE_STATE_VALUE_END;
                        index--;
                    } else state = SERIALISABLE_STATE_ERROR;

                    break;

                case '\'':
                case '"':

                    // Ignore quote if it isn't a string terminator
                    if (in_string && c != 0 && c != in_string_char) {
                        index++;
                        continue;
                    }

                    // We're either going into a string or out of one
                    in_string = !in_string;

                    if (in_string == false) {
                        in_string_char = 0;
                    } else {
                        in_string_char = c;
                    }

                    if (depth_object > 1) {
                        index++;
                        continue;
                    }

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_OBJECT_START) state = SERIALISABLE_STATE_NAME_START;
                    else if (state == SERIALISABLE_STATE_COMMA) state = SERIALISABLE_STATE_NAME_START;
                    else if (state == SERIALISABLE_STATE_NAME_START) state = SERIALISABLE_STATE_NAME_END;
                    else if (state == SERIALISABLE_STATE_SEPARATOR) {
                        state = SERIALISABLE_STATE_VALUE_START;
                        value_type = SERIALISABLE_VALUE_TYPE_STRING;
                    } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_END;
                    else state = SERIALISABLE_STATE_ERROR;

                    break;

                case ':':
                    if (depth_object > 1) {
                        index++;
                        continue;
                    }

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in string value
                    else if (state == SERIALISABLE_STATE_NAME_END) state = SERIALISABLE_STATE_SEPARATOR;
                    else state = SERIALISABLE_STATE_ERROR;

                    break;

                case ',':
                    if (depth_object > 1) {
                        index++;
                        continue;
                    }

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in string value
                    else if (state == SERIALISABLE_STATE_VALUE_END) state = SERIALISABLE_STATE_COMMA;
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NUM) {
                        state = SERIALISABLE_STATE_VALUE_END;
                        index--;
                    } else if (state == SERIALISABLE_STATE_OBJECT_END) state = SERIALISABLE_STATE_COMMA;
                    else state = SERIALISABLE_STATE_ERROR;

                    break;

                case '[':
                    if (depth_object > 1) {
                        index++;
                        continue;
                    }

                    depth_array++;

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in string value
                    else if (state == SERIALISABLE_STATE_SEPARATOR) {
                        state = SERIALISABLE_STATE_VALUE_START;
                        value_type = SERIALISABLE_VALUE_TYPE_ARRAY;
                    } else state = SERIALISABLE_STATE_ERROR;

                    break;

                case ']':
                    if (depth_object > 1) {
                        index++;
                        continue;
                    }

                    depth_array--;

                    if (depth_array > 0) {
                        index++;
                        continue;
                    }

                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_END; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_END; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in string value
                    else state = SERIALISABLE_STATE_ERROR;

                    break;

                default:
                    if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NULL) {
                        state = SERIALISABLE_STATE_VALUE_END;
                        index += 2;
                    } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_BOOL_FALSE) {
                        state = SERIALISABLE_STATE_VALUE_END;
                        index += 3;
                    } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_BOOL_TRUE) {
                        state = SERIALISABLE_STATE_VALUE_END;
                        index += 2;
                    } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                    else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in object value
                    else if (state == SERIALISABLE_STATE_OBJECT_START && isValidNameCharStart(c)) state = SERIALISABLE_STATE_NAME_START;
                    else if (state == SERIALISABLE_STATE_NAME_START && isValidNameChar(c)) state = SERIALISABLE_STATE_NAME_START;
                    else if (state == SERIALISABLE_STATE_SEPARATOR && isNumberChar(c)) {
                        state = SERIALISABLE_STATE_VALUE_START;
                        value_type = SERIALISABLE_VALUE_TYPE_NUM;
                    } else if (state == SERIALISABLE_STATE_VALUE_START) state = SERIALISABLE_STATE_VALUE_START;
                    else if (!isEmptySpaceChar(c)) state = SERIALISABLE_STATE_ERROR;
                }
            }
        }

#ifdef _DEBUG_JSON_STATE
        printf("%c: Object State: c:=%d state:=%d value_type:=%zu depth_array:=%zu in_string:=%d in_string_char:=%c\n", c, state, value_type, depth_object, depth_array, in_string, in_string_char);
#endif

        if (state == SERIALISABLE_STATE_ERROR) {
            printf("PARSE SERIALISABLE_STATE_ERROR (Object):\n%s\n", s);
            size_t n = index;

            while (n-- > 0) printf(" ");

            printf("^ Character: %zd:%s\n", index, &buffer[index - 5]);
            return false;
        }

        if (previous_state != state) {
            switch (state) {
            case SERIALISABLE_STATE_NAME_START:
            case SERIALISABLE_STATE_VALUE_START:
                pos_start = index;
                break;

            case SERIALISABLE_STATE_NAME_END:
                pos_end = index;
                name = substring(buffer, pos_start + 1, pos_end);

                if (name) {
#ifdef _DEBUG_JSON
                    printf("Name: %s\n", name);
#endif
                }

                break;

            case SERIALISABLE_STATE_VALUE_END:
                pos_end = index;

                switch (value_type) {
                case SERIALISABLE_VALUE_TYPE_NUM:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                case SERIALISABLE_VALUE_TYPE_STRING:
                    value = substring(buffer, pos_start + 1, pos_end);
                    break;

                case SERIALISABLE_VALUE_TYPE_ARRAY:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                case SERIALISABLE_VALUE_TYPE_OBJECT:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                case SERIALISABLE_VALUE_TYPE_NULL:
                    value = NULL;
                    break;

                case SERIALISABLE_VALUE_TYPE_BOOL_FALSE:
                case SERIALISABLE_VALUE_TYPE_BOOL_TRUE:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                default:
                    value = NULL;
                    break;
                }

                if (name && value) {
#ifdef _DEBUG_JSON
                    printf("Value: %s\n\n", value);
#endif
                    // Remove duplicate entry
                    map_item* existing_item = map_get_item(deserialised_attrs, name);

                    if (existing_item) {
                        printf("\n!!!!WARNING!!!!: Detected duplicate JSON name entry: %s in \n%s\n\n", name, s);
                        xmemory_free(name);

                        if (value) {
                            xmemory_free(value);
                        }

                        //if (existing_item->value)
                        //{
                        //	text_free(existing_item->value);
                        //}
//						assert(false);
                        return false;
                    }

                    map_set(deserialised_attrs, name, value);
                }

                if (name) {
                    xmemory_free(name);
                    name = NULL;
                }

                break;

            default:
                break;
            }

            previous_state = state;
        }

        index++;
    }

    return true;
}

bool json_deserialise_array(char* s, list* deserialised_list) {
    if (text_equals(s, "[]")) {
        return true;
    }

    DeserialiserState state = SERIALISABLE_STATE_INIT;
    DeserialiserState previous_state = SERIALISABLE_STATE_INIT;

    DeserialiserValueType value_type = SERIALISABLE_VALUE_TYPE_UNKNOWN;
    bool in_string = false;		// Are we in a string
    char in_string_char = 0;	// How did the string start?

    const char* buffer = s;
    size_t length = strlen(s);
    size_t index = 0;
    size_t pos_start = 0;
    size_t pos_end = 0;

    char* value = NULL;

    int depth_array = 0;
    int depth_object = 0;

    while (index < length) {
        char c = buffer[index];

        if (c == '\\') {// && state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) {
            state = SERIALISABLE_STATE_VALUE_START;
            index += 2;
            //			c = buffer[index];
            continue;
        } else {
            switch (c) {
            case '[':
                depth_array++;

                if (state == SERIALISABLE_STATE_INIT) state = SERIALISABLE_STATE_ARRAY_START;
                else if (state == SERIALISABLE_STATE_VALUE_START) state = SERIALISABLE_STATE_VALUE_START;
                else state = SERIALISABLE_STATE_ERROR;

                break;

            case ']':
                depth_array--;

                if (depth_array > 0) {
                    index++;
                    continue;
                }

                if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NUM) {
                    state = SERIALISABLE_STATE_VALUE_END;
                } else if (state == SERIALISABLE_STATE_VALUE_END) state = SERIALISABLE_STATE_VALUE_END;
                else state = SERIALISABLE_STATE_ERROR;

                break;

            case '{':
                depth_object++;

                if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                else if (state == SERIALISABLE_STATE_ARRAY_START) {
                    state = SERIALISABLE_STATE_VALUE_START;
                    value_type = SERIALISABLE_VALUE_TYPE_OBJECT;
                } else if (state == SERIALISABLE_STATE_COMMA) {
                    state = SERIALISABLE_STATE_VALUE_START;
                    value_type = SERIALISABLE_VALUE_TYPE_OBJECT;
                } else state = SERIALISABLE_STATE_ERROR;

                break;

            case '}':
                depth_object--;

                if (depth_object > 0) {
                    index++; continue;
                }

                if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_ARRAY) state = SERIALISABLE_STATE_VALUE_START; // Capture everything in array value
                else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_END; // Capture everything in object value
                else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NUM) {
                    state = SERIALISABLE_STATE_VALUE_END;
                    index--;
                } else if (state == SERIALISABLE_STATE_VALUE_END) state = SERIALISABLE_STATE_OBJECT_END;
                else state = SERIALISABLE_STATE_ERROR;

                break;

            case '"':
            case '\'':

                // Ignore quote if it isn't a string terminator
                if (in_string && c != 0 && c != in_string_char) {
                    index++;
                    continue;
                }

                // We're either going into a string or out of one
                in_string = !in_string;

                if (in_string == false) {
                    in_string_char = 0;
                } else {
                    in_string_char = c;
                }

                if (state == SERIALISABLE_STATE_ARRAY_START) {
                    state = SERIALISABLE_STATE_VALUE_START;
                    value_type = SERIALISABLE_VALUE_TYPE_STRING;
                } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_STRING) {
                    state = SERIALISABLE_STATE_VALUE_END;
                } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) {
                    state = SERIALISABLE_STATE_VALUE_START;
                } else if (state == SERIALISABLE_STATE_COMMA) {
                    state = SERIALISABLE_STATE_VALUE_START;
                    value_type = SERIALISABLE_VALUE_TYPE_STRING;
                } else state = SERIALISABLE_STATE_ERROR;

                break;


            case ',':
                if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NUM) {
                    state = SERIALISABLE_STATE_VALUE_END;
                    index--;
                } else if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_OBJECT) state = SERIALISABLE_STATE_VALUE_START;
                else if (state == SERIALISABLE_STATE_VALUE_END) state = SERIALISABLE_STATE_COMMA;
                else state = SERIALISABLE_STATE_ERROR;

                break;

            default:
                if (state == SERIALISABLE_STATE_VALUE_START && value_type == SERIALISABLE_VALUE_TYPE_NUM && isEmptySpaceChar(c)) {
                    state = SERIALISABLE_STATE_VALUE_END;
                    index--;
                } else if (state == SERIALISABLE_STATE_VALUE_START) state = SERIALISABLE_STATE_VALUE_START;
                else if (state == SERIALISABLE_STATE_ARRAY_START && isNumberChar(c)) {
                    state = SERIALISABLE_STATE_VALUE_START;
                    value_type = SERIALISABLE_VALUE_TYPE_NUM;
                } else if (state == SERIALISABLE_STATE_COMMA && isNumberChar(c)) {
                    state = SERIALISABLE_STATE_VALUE_START;
                    value_type = SERIALISABLE_VALUE_TYPE_NUM;
                } else if (!isEmptySpaceChar(c)) state = SERIALISABLE_STATE_ERROR;
            }
        }

#ifdef _DEBUG_JSON_STATE
        printf("%c: Array State: c:=%d state:=%d value_type:=%zu depth_array:=%zu in_string:=%d in_string_char:=%c\n", c, state, value_type, depth_object, depth_array, in_string, in_string_char);
#endif

        if (state == SERIALISABLE_STATE_ERROR) {
            printf("PARSE SERIALISABLE_STATE_ERROR (Array):\n%s\n", s);
            size_t n = index;

            while (n-- > 0) printf(" ");

            printf("^ Character: %zd:%s\n", index, &buffer[index - 5]);
            return false;
        }

        if (previous_state != state) {
            switch (state) {
            case SERIALISABLE_STATE_VALUE_START:
                pos_start = index;
                break;

            case SERIALISABLE_STATE_VALUE_END:
                pos_end = index;

                switch (value_type) {
                case SERIALISABLE_VALUE_TYPE_NUM:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                case SERIALISABLE_VALUE_TYPE_STRING:
                    value = substring(buffer, pos_start + 1, pos_end);
                    break;

                case SERIALISABLE_VALUE_TYPE_ARRAY:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                case SERIALISABLE_VALUE_TYPE_OBJECT:
                    value = substring(buffer, pos_start, pos_end + 1);
                    break;

                default:
                    value = NULL;
                    break;
                }

                if (value) {
#ifdef _DEBUG_JSON
                    printf("Value: %s\n", value);
#endif
                    list_add(deserialised_list, value);
                }

                break;

            default:
                break;
            }

            previous_state = state;
        }


        index++;
    }

    return true;
}

bool json_deserialise_map(char* s, map* deserialised_map) {
    return json_deserialise_object(s, deserialised_map);
}

#pragma endregion Parser