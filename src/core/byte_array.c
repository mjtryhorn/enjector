/*
* byte_array.c
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

#include <enjector/core/byte_array.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/hex.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <limits.h>

#define DEFAULT_STREAM_CAPACITY 32

byte_array* _byte_array_create(const unsigned char* data, size_t length, const char* filename, unsigned int line) {
    byte_array* ba = (byte_array*) _xmemory_malloc(sizeof(byte_array), filename, line);
    ba->length = length;
    ba->stream_capacity = 0;

    if (data == NULL || length == 0) {
        ba->data = 0;
    } else {
        ba->data = (unsigned char*) _xmemory_malloc(length, filename, line);
        memmove(ba->data, data, length);
    }

    return ba;
}

byte_array* _byte_array_create_empty(size_t length, const char* filename, unsigned int line) {
    byte_array* ba = (byte_array*) _xmemory_malloc(sizeof(byte_array), filename, line);
    ba->length = length;
    ba->stream_capacity = 0;

    if (length > 0) {
        ba->data = (unsigned char*)_xmemory_malloc(length, filename, line);
    }

    return ba;
}

byte_array*	_byte_array_create_from_hex(const char* hex, const char* filename, unsigned int line) {
    assert(hex);

    const size_t hex_len = strlen(hex);

    const unsigned char* data = hex_to_bin(hex, hex_len);

    if (!data) {
        return NULL;
    }

    byte_array* ba = (byte_array*) _xmemory_malloc(sizeof(byte_array), filename, line);
    ba->length = hex_len / 2;
    ba->stream_capacity = 0;
    ba->data = (unsigned char*) data;

    return ba;
}

/*byte_array*	_byte_array_create_from_base64(const char* base64, const char* filename, unsigned int line)
{
	assert(base64);

	size_t _len = strlen(hex);

	const unsigned char* data = hex_to_bin(base64, base64_len);
	if (!data)
	{
		return NULL;
	}

	byte_array* ba;

	ba = (byte_array*) _xmemory_malloc(sizeof(byte_array), filename, line);
	ba->length = hex_len / 2;
	ba->stream_capacity = 0;
	ba->data = data;

	return ba;
}
*/

void byte_array_free(byte_array* ba) {
    assert(ba);

    if (ba->data) {
        xmemory_free(ba->data);
    }

    xmemory_free(ba);
}

byte_array* _byte_array_create_stream(const char* filename, unsigned int line) {
    byte_array* ba = (byte_array*) _xmemory_malloc(sizeof(byte_array), filename, line);
    ba->data = (unsigned char*) _xmemory_malloc(DEFAULT_STREAM_CAPACITY, filename, line);

    ba->length = 0;
    ba->stream_capacity = DEFAULT_STREAM_CAPACITY;

    return ba;
}

void byte_array_append(byte_array* dst, byte_array* src, size_t offset, size_t length) {
    assert(dst);
    assert(dst->data);
    assert(src);
    assert(src->data);
    assert(src->length);
    assert(offset < src->length);
    assert((offset + length) <= src->length);

    // Check if destination is a stream
    if (dst->stream_capacity == 0) {
        printf("byte_array not a stream");
        assert(0);
    }

    if (length == 0) {
        return;
    }

    // Check if there's enough capacity in the stream, resize otherwise
    if ((dst->length + length) > dst->stream_capacity) {

        // Resize
        dst->stream_capacity += length + DEFAULT_STREAM_CAPACITY;
        unsigned char* resized_buffer = (unsigned char*) xmemory_malloc(dst->stream_capacity);
        memmove(resized_buffer, dst->data, dst->length);
        xmemory_free(dst->data);
        dst->data = resized_buffer;
    }

    // Append
    memmove(&dst->data[dst->length], &src->data[offset], length);
    dst->length += length;
}

void byte_array_append_raw(byte_array* dst, const unsigned char* data, size_t offset, size_t length) {
    assert(dst);
    assert(dst->data);
    assert(data);
    assert(length > 0);

    // Check if destination is a stream
    if (dst->stream_capacity == 0) {
        printf("byte_array not a stream");
        assert(0);
    }

    if (length == 0) {
        return;
    }

    // Check if there's enough capacity in the stream, resize otherwise
    if ((dst->length + length) > dst->stream_capacity ) {

        // Resize
        dst->stream_capacity += length + DEFAULT_STREAM_CAPACITY;
        unsigned char* resized_buffer = (unsigned char*) xmemory_malloc(dst->stream_capacity);
        memmove(resized_buffer, dst->data, dst->length);
        xmemory_free(dst->data);
        dst->data = resized_buffer;
    }

    // Append
    memmove(&dst->data[dst->length], &data[offset], length);
    dst->length += length;
}

void byte_array_copy(byte_array* dst, unsigned int dst_index, byte_array* src, unsigned int src_index, size_t length) {
    assert(dst);
    assert(dst_index < dst->length);
    assert(src);
    assert(src_index  < src->length);
    assert((src_index + length) <= src->length);

    memcpy(&dst->data[dst_index], &src->data[src_index], length);
}

void byte_array_append_byte(byte_array* dst, unsigned char b) {
    unsigned char data[1];
    byte_array src;

    assert(dst);

    // Check if destination is a stream
    if (dst->stream_capacity == 0) {
        printf("byte_array not a stream");
        assert(0);
    }

    data[0] = b;
    src.data = data;
    src.length = 1;
    src.stream_capacity = 0;

    byte_array_append(dst, &src, 0, 1);
}

int byte_array_is_equal(byte_array* a, byte_array* b) {
    assert(a);
    assert(a->data);
    assert(b);
    assert(b->data);

    if (a->length != b->length) {
        return 0;
    }

    for (int i = 0; i < a->length; i++) {
        if (a->data[i] != b->data[i]) {
            return 0;
        }
    }

    return 1;
}

int byte_array_is_equal_raw(byte_array* a, unsigned char* b_data, size_t b_length) {
    assert(a);
    assert(a->data);
    assert(b_data);

    if (a->length != b_length) {
        return 0;
    }

    for (int i = 0; i < a->length; i++) {
        if (a->data[i] != b_data[i]) {
            return 0;
        }
    }

    return 1;
}

void byte_array_print(const char* prefix, byte_array* ba) {
    hex_print(prefix, ba->data, ba->length);
}


// TODO: remove
void byte_array_file_write_hex(const char* filename, byte_array* ba) {
    assert(filename);
    assert(ba);

    char* data = hex_from_bin(ba->data, ba->length);
    FILE* file = fopen(filename, "w");
    fwrite(data, 1, ba->length* 2, file);
    fclose(file);

    hex_free(data);
}

char* byte_array_to_hex(byte_array* ba) {
    assert(ba);

    return hex_from_bin(ba->data, ba->length);
}

byte_array* byte_array_clone(byte_array* src) {
    assert(src);

    byte_array* dst = byte_array_create(src->data, src->length);

    return dst;
}

void byte_array_reverse(byte_array* src) {
    assert(src);

    byte_array* dst = byte_array_create_empty(src->length);
    int i = 0;
    size_t n = src->length;

    while(n-- > 0) {
        dst->data[i++] = src->data[n];
    }

    byte_array_copy(src, 0, dst, 0, src->length);
    byte_array_free(dst);
}

byte_array* byte_array_rotate_left_with_overflow(byte_array* source) {
    assert(source);

    byte_array* dest = byte_array_clone(source);
    unsigned char* byte = dest->data;
    size_t size = dest->length;

    // Do we have a leading bit?
    const unsigned int has_leading_bit = *byte & 0x80;

    for (byte = dest->data; size--; ++byte) {
        unsigned char bit = 0;

        if (size) {
            // Grab the high bit of the next byte
            bit = byte[1] & (1 << (CHAR_BIT - 1)) ? 1 : 0;
        }

        *byte <<= 1;   // Shift the current byte
        *byte  |= bit; // Bring in the high bit of the next byte
    }

    // Perform XOR if most significant bit was set
    if (has_leading_bit) {
        dest->data[dest->length - 1] ^= 0x87;
    }

    return dest;
}

byte_array*	byte_array_xor(byte_array* source1, byte_array* source2) {
    assert(source1);
    assert(source2);

    byte_array* dest = byte_array_clone(source1);
    const size_t size = dest->length;

    for(unsigned int index = 0; index < size; index++) {
        dest->data[index] = source1->data[index] ^ source2->data[index];
    }

    return dest;
}
