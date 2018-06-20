/*
* hex.c
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

#include <enjector/core/hex.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include <string.h>

char* hex_from_bin(unsigned char* bin, size_t bin_len) {
    char hex_lookup[] = "0123456789abcdef";

	if (bin == 0 || bin_len == 0) {
        return NULL;
    }

    char * hex_from_bin_buffer = (char*)xmemory_malloc(bin_len * 2 + 1);
    memset(hex_from_bin_buffer, 0, bin_len * 2 + 1);

    for (size_t i = 0; i < bin_len; i++) {
        hex_from_bin_buffer[i * 2 + 0] = hex_lookup[(bin[i] >> 4) & 0x0F];
        hex_from_bin_buffer[i * 2 + 1] = hex_lookup[bin[i] & 0x0F];
    }

    return hex_from_bin_buffer;
}

static int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'a' && c <= 'f') {
        return (c - 'a') + 10;
    }

    if (c >= 'A' && c <= 'F') {
        return (c - 'A') + 10;
    }

    assert(0);

    return 0;
}

unsigned char* hex_to_bin(const char* hex, size_t hex_len) {
    size_t j = 0;

	if (hex == 0 || hex_len == 0) {
        return 0;
    }

    if (hex_len % 2 != 0) {
        printf("Invalid hex string");
        assert(0);
    }

    unsigned char * hex_to_bin_buffer = (unsigned char*)xmemory_malloc(hex_len / 2 + 1);

    memset(hex_to_bin_buffer, 0, hex_len / 2 + 1);

    for (size_t i = 0; i < hex_len; i += 2) {
	    const int x = hex_char_to_int(hex[i]);
	    const int y = hex_char_to_int(hex[i + 1]);

        hex_to_bin_buffer[j++] = x * 16 + y;
    }

    return hex_to_bin_buffer;
}

size_t hex_to_unsigned_integer(const char* hex) {
    size_t result = 0;
    int ratio = 1;
    size_t i = text_length(hex);

    while (i-- > 0) {
        result += hex_char_to_int(hex[i]) * ratio;
        ratio *= 16;
    }

    return result;
}

unsigned long long hex_to_unsigned_long_long(const char* hex) {
    unsigned long long result = 0;
    int ratio = 1;
    size_t i = text_length(hex);

    while (i-- > 0) {
        result += hex_char_to_int(hex[i]) * ratio;
        ratio *= 16;
    }

    return result;
}

void hex_print(const char* prefix, unsigned char* bin, size_t bin_len) {
	assert(bin);
    assert(bin_len > 0);

    if (prefix != NULL) {
        printf("%s (%zd): ", prefix, bin_len);
    }

    for (int i = 0; i < bin_len; i++) {
        printf("%02x", bin[i]);
    }

    printf("\n");
}

void hex_free(void* ptr) {
    assert(ptr);
    xmemory_free(ptr);
}
