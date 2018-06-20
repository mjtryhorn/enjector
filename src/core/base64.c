/*
* base64.c
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

#include <assert.h>
#include <inttypes.h>
#include <string.h>

byte_array* base64_encode(byte_array* input) {
    assert(input);

    void* data_buf = input->data;
	const size_t dataLength = input->length;

    byte_array* result = byte_array_create_stream();

    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t* data = (const uint8_t* )data_buf;
	uint32_t n = 0;
    int pad_count = dataLength % 3;

	// Increment over the length of the string, three characters at a time
    for (size_t x = 0; x < dataLength; x += 3) {
        // These three 8-bit (ASCII) characters become one 24-bit number
        n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if ((x + 1) < dataLength)
            n += ((uint32_t)data[x + 1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if ((x + 2) < dataLength)
            n += data[x + 2];

        // This 24-bit number gets separated into four 6-bit numbers
	    const uint8_t n0 = (uint8_t)(n >> 18) & 63;
        const uint8_t n1 = (uint8_t)(n >> 12) & 63;
        const uint8_t n2 = (uint8_t)(n >> 6) & 63;
        const uint8_t n3 = (uint8_t)n & 63;

        // If we have one byte available, then its encoding is spread out over two characters
        byte_array_append_byte(result, base64chars[n0]);
        byte_array_append_byte(result, base64chars[n1]);

        // If we have only two bytes available, then their encoding is spread out over three chars
        if ((x + 1) < dataLength) {
            byte_array_append_byte(result, base64chars[n2]);
        }

        // if we have all three bytes available, then their encoding is spread out over four characters
        if ((x + 2) < dataLength) {
            byte_array_append_byte(result, base64chars[n3]);
        }
    }

    // Create and add padding that is required if we did not have a multiple of number of characters available
    if (pad_count > 0) {
        for (; pad_count < 3; pad_count++) {
            byte_array_append_byte(result, '=');
        }
    }

    byte_array_append_byte(result, 0);

    return result;
}

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const unsigned char d[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

byte_array* base64_decode(byte_array* input) {
    assert(input);

    byte_array* result = byte_array_create_stream();

    unsigned char* in = input->data;
	const size_t inLen = input->length;

    //unsigned char* out, size_t* outLen) {
    unsigned char* end = in + inLen;
    char iter = 0;
    size_t buf = 0;

    while (in < end) {
        unsigned char c = d[*in++];

        switch (c) {
        case WHITESPACE: continue;		// Skip whitespace

        case INVALID:
            return NULL;				// Invalid input, return error

        case EQUALS:					// Pad character, end of data
            in = end;
            continue;

        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration

            // If the buffer is full, split it into bytes
            if (iter == 4) {
                byte_array_append_byte(result, (buf >> 16) & 255);
                byte_array_append_byte(result, (buf >> 8) & 255);
                byte_array_append_byte(result, buf & 255);
                buf = 0; iter = 0;
            }
        }
    }

    if (iter == 3) {
        byte_array_append_byte(result, (buf >> 10) & 255);
        byte_array_append_byte(result, (buf >> 2) & 255);
    } else if (iter == 2) {
        byte_array_append_byte(result, (buf >> 4) & 255);
    }

    return result;
}
