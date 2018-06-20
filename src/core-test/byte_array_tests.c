/*
* byte_array_tests.c
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
#include "byte_array_tests.h"

#include <enjector/core/byte_array.h>
#include <enjector/core/hex.h>
#include <enjector/core/xmemory.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void should_successfully_create_byte_array_with_data() {
    const char* expected_data = "Hello World 1234567890";
    const size_t expected_data_length = strlen(expected_data);

    byte_array* ba = byte_array_create((unsigned char*) expected_data, expected_data_length);

    TEST_ASSERT_EQUAL_SIZE(ba->length, expected_data_length);
    TEST_ASSERT_TRUE(!memcmp((char*) ba->data, expected_data, expected_data_length));

    byte_array_free(ba);

    xmemory_report_exit_on_leaks();
}

static void should_succeed_if_data_length_is_0() {
    const char* expected_data = "";
    const int expected_data_length = 0;

    byte_array* ba = byte_array_create((unsigned char*) expected_data, expected_data_length);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, 0);
    TEST_ASSERT_PTR_NULL(ba->data);

    byte_array_free(ba);
    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_empty_byte_array_with_data() {
    const int expected_data_length = 100;

    byte_array* ba = byte_array_create_empty(expected_data_length);

    TEST_ASSERT_EQUAL_SIZE(ba->length, expected_data_length);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);

    byte_array_free(ba);
    xmemory_report_exit_on_leaks();
}

static void should_succeed_to_create_stream_with_some_capacity() {
    byte_array* ba = byte_array_create_stream();

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, 0);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);
    TEST_ASSERT_TRUE(ba->stream_capacity > 0);

    byte_array_free(ba);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_append_a_single_subset_of_bytes() {
    unsigned char source[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    const size_t length = 4;

    byte_array* ba_source = byte_array_create(source, sizeof(source));
    byte_array* ba = byte_array_create_stream();

    byte_array_append(ba, ba_source, 0, length);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, length);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);
    TEST_ASSERT_TRUE(ba->stream_capacity > 0);

    for (size_t i = 0; i < length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[i], source[i]);
    }

    byte_array_free(ba_source);
    byte_array_free(ba);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_append_a_multiple_subset_of_bytes() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char source2[] = {
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    const size_t length = 8;
    size_t i;

    byte_array* ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array* ba_source2 = byte_array_create(source2, sizeof(source2));
    byte_array* ba = byte_array_create_stream();

    byte_array_append(ba, ba_source1, 0, length);
    byte_array_append(ba, ba_source2, 0, length);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, length* 2);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);
    TEST_ASSERT_TRUE(ba->stream_capacity > 0);

    //byte_array_print("Source1", ba_source1);
    //byte_array_print("Source2", ba_source2);
    //byte_array_print("Stream", ba);

    for (i = 0; i < length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[i], source1[i]);
    }

    for (i = 0; i < length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[length + i], source2[i]);
    }

    byte_array_free(ba);
    byte_array_free(ba_source1);
    byte_array_free(ba_source2);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_append_a_large_subset_of_bytes_and_resize() {
    unsigned char source[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    const unsigned int source_length = sizeof(source);

    size_t i;

    byte_array* ba_source = byte_array_create(source, source_length);
    byte_array* ba = byte_array_create_stream();

    const size_t old_capacity = ba->stream_capacity;

    // Append multiple times (capacity is 32 by default)
    byte_array_append(ba, ba_source, 0, source_length);
    byte_array_append(ba, ba_source, 0, source_length);
    byte_array_append(ba, ba_source, 0, source_length);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, source_length* 3);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);
    TEST_ASSERT_TRUE(ba->stream_capacity > old_capacity);

    for (i = 0; i < source_length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[i], source[i]);
    }

    for (i = 0; i < source_length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[source_length + i], source[i]);
    }

    for (i = 0; i < source_length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[source_length* 2 + i], source[i]);
    }

    byte_array_free(ba);
    byte_array_free(ba_source);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_copy_subset() {
    unsigned char source[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    const size_t length = 12;

    byte_array* ba_source = byte_array_create(source, sizeof(source));
    byte_array* ba = byte_array_create_empty(100);

    byte_array_copy(ba, 10, ba_source, 4, length);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, 100);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);
    TEST_ASSERT_EQUAL_SIZE(ba->stream_capacity, 0); // Not a stream

    for (size_t i = 0; i < length; i++) {
        TEST_ASSERT_EQUAL_SIZE(ba->data[10 + i], source[4 + i]);
    }

    byte_array_free(ba_source);
    byte_array_free(ba);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_byte_arrays1() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char source2[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array* ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array* ba_source2 = byte_array_create(source2, sizeof(source2));

    const int result = byte_array_is_equal(ba_source1, ba_source2);

    TEST_ASSERT_EQUAL_SIZE(result, 1);

    byte_array_free(ba_source1);
    byte_array_free(ba_source2);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_byte_arrays2() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x40, 0x41, 0x42,
        0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
        0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char source2[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x40, 0x41, 0x42,
        0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
        0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array* ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array* ba_source2 = byte_array_create(source2, sizeof(source2));

    const int result = byte_array_is_equal(ba_source1, ba_source2);

    TEST_ASSERT_EQUAL_SIZE(result, 1);

    byte_array_free(ba_source1);
    byte_array_free(ba_source2);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_byte_arrays3() {
    unsigned char source1[] = {0x40};
    unsigned char source2[] = {0x40};

    byte_array* ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array* ba_source2 = byte_array_create(source2, sizeof(source2));

    const int result = byte_array_is_equal(ba_source1, ba_source2);

    TEST_ASSERT_EQUAL_SIZE(result, 1);

    byte_array_free(ba_source1);
    byte_array_free(ba_source2);

    xmemory_report_exit_on_leaks();
}

static void should_fail_compare_byte_arrays1() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x99
    };
    unsigned char source2[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array* ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array* ba_source2 = byte_array_create(source2, sizeof(source2));

    const int result = byte_array_is_equal(ba_source1, ba_source2);

    TEST_ASSERT_EQUAL_SIZE(result, 0);

    byte_array_free(ba_source1);
    byte_array_free(ba_source2);

    xmemory_report_exit_on_leaks();
}

static void should_fail_compare_byte_arrays2() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x99, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    unsigned char source2[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array * ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array * ba_source2 = byte_array_create(source2, sizeof(source2));

    const int result = byte_array_is_equal(ba_source1, ba_source2);

    TEST_ASSERT_EQUAL_SIZE(result, 0);

    byte_array_free(ba_source1);
    byte_array_free(ba_source2);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_compare_byte_array_to_raw_data() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char source2[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array * ba_source1 = byte_array_create(source1, sizeof(source1));

    const int result = byte_array_is_equal_raw(ba_source1, source2, sizeof(source2));

    TEST_ASSERT_EQUAL_SIZE(result, 1);

    byte_array_free(ba_source1);

    xmemory_report_exit_on_leaks();
}

static void should_fail_compare_byte_array_to_raw_data() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x99, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char source2[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array * ba_source1 = byte_array_create(source1, sizeof(source1));

    const int result = byte_array_is_equal_raw(ba_source1, source2, sizeof(source2));

    TEST_ASSERT_EQUAL_SIZE(result, 0);

    byte_array_free(ba_source1);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_append_singe_byte() {
    byte_array * ba = byte_array_create_stream();

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, 0);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba->data);
    TEST_ASSERT_TRUE(ba->stream_capacity > 0);

    byte_array_append_byte(ba, 123);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(ba);
    TEST_ASSERT_EQUAL_SIZE(ba->length, 1);
    TEST_ASSERT_TRUE(ba->stream_capacity > 1);
    TEST_ASSERT_EQUAL_SIZE(ba->data[0], 123);

    byte_array_free(ba);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_append_raw_data() {
    unsigned char source1[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x99, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char source2[] = {
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f
    };
    unsigned char expected_data[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x99, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52,
        0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f
    };

    byte_array * ba_stream = byte_array_create_stream();
    byte_array_append_raw(ba_stream, source1, 0, sizeof(source1));
    byte_array_append_raw(ba_stream, source2, 0, sizeof(source2));

    byte_array * ba_expected_data = byte_array_create(expected_data, sizeof(expected_data));
    const int result = byte_array_is_equal(ba_stream, ba_expected_data);

    TEST_ASSERT_EQUAL_SIZE(result, 1);

    byte_array_free(ba_stream);
    byte_array_free(ba_expected_data);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_reversed_data() {
    unsigned char source[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    unsigned char expected_data[] = {
        0x4f, 0x4e, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x40
    };

    byte_array * ba_expected = byte_array_create(expected_data, sizeof(expected_data));
    byte_array * ba_reversed = byte_array_create(source, sizeof(source));
    byte_array_reverse(ba_reversed);
    const int result = byte_array_is_equal(ba_reversed, ba_expected);

    //byte_array_print("Reversed", ba_reversed);
    //byte_array_print("Expected", ba_expected);

    TEST_ASSERT_EQUAL_SIZE(result, 1);

    byte_array_free(ba_expected);
    byte_array_free(ba_reversed);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_dump_data() {
    unsigned char source[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    char dumped[2048];

    char * source_hex = hex_from_bin(source, sizeof(source));

    byte_array * ba_source = byte_array_create(source, sizeof(source));
    byte_array_file_write_hex("output.txt", ba_source);

    // read the hex data
    FILE * dumped_file = fopen("output.txt", "r");
    const size_t dumped_len = fread(dumped, 1, sizeof(source) * 2, dumped_file);
    dumped[dumped_len] = 0;
    fclose(dumped_file);

    TEST_ASSERT_TRUE(!(strcmp(dumped, source_hex)));

    hex_free(source_hex);
    byte_array_free(ba_source);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_left_shift_bits1() {
    //								11110000	11001100	10101010	11110000
    unsigned char source[] = {0xF0, 0xCC, 0xAA, 0xF0};
    //								11100001	10011001	01010101	11100000 ^ 0x87
    unsigned char expected[] = {0xE1, 0x99, 0x55, 0xe0 ^ 0x87};

    byte_array* ba_source = byte_array_create(source, sizeof(source));
    byte_array* ba_dest = byte_array_rotate_left_with_overflow(ba_source);

    TEST_ASSERT_TRUE(!(strncmp((const char*) ba_dest->data, (const char*) expected, sizeof(expected))));

    byte_array_free(ba_source);
    byte_array_free(ba_dest);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_left_shift_bits2() {
    // 11100001 .... 00101011
    unsigned char source[] = {
        0xE1, 0x6C, 0x70, 0x6F, 0xF8, 0x2F, 0x2B, 0x3A, 0xD5, 0x21, 0xC7, 0xA2, 0xF9, 0xFB, 0xD5, 0x2B
    };

    // 11000010 .... 11010001
    //				 01010110
    unsigned char expected[] = {
        0xC2, 0xD8, 0xE0, 0xDF, 0xF0, 0x5E, 0x56, 0x75, 0xAA, 0x43, 0x8F, 0x45, 0xF3, 0xF7, 0xAA, 0xD1
    };

    byte_array* ba_source = byte_array_create(source, sizeof(source));
    byte_array* ba_dest = byte_array_rotate_left_with_overflow(ba_source);

    TEST_ASSERT_TRUE(!(strncmp((const char*) ba_dest->data, (const char*) expected, sizeof(expected))));

    //byte_array_print("ok", ba_dest);

    byte_array_free(ba_source);
    byte_array_free(ba_dest);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_xor_bits() {
    //								11110000	11001100	10101010	11110000
    unsigned char source1[] = {0xF0, 0xCC, 0xAA, 0xF0};
    //								10101100	01100001	11110001	10101010
    unsigned char source2[] = {0xAC, 0x61, 0xF1, 0xAA};
    //								01011100 	10101101	01011011	1011010
    unsigned char expected[] = {0x5C, 0xAD, 0x5B, 0x5A};

    byte_array* ba_source1 = byte_array_create(source1, sizeof(source1));
    byte_array* ba_source2 = byte_array_create(source2, sizeof(source2));
    byte_array* ba_xor = byte_array_xor(ba_source1, ba_source2);

    TEST_ASSERT_TRUE(!(strncmp((const char*) ba_xor->data, (const char*) expected, sizeof(expected))));

    byte_array_free(ba_source1);
    byte_array_free(ba_source2);
    byte_array_free(ba_xor);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_byte_array_from_hex() {
    const char* input = "404142434445464748494a4b4c4d4e4f";
    unsigned char expected_value[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };

    byte_array* actual_value = byte_array_create_from_hex(input);

    TEST_ASSERT_TRUE_FATAL(byte_array_is_equal_raw(actual_value, expected_value, sizeof(expected_value)));

    byte_array_free(actual_value);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_hex_from_byte_array() {
    unsigned char input[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
    };
    const char* expected_value = "404142434445464748494a4b4c4d4e4f";

    byte_array* input_value = byte_array_create(input, sizeof(input));
    char* actual_value = byte_array_to_hex(input_value);

    TEST_ASSERT_TRUE_FATAL(!strcmp(actual_value, expected_value));

    byte_array_free(input_value);
    hex_free(actual_value);

    xmemory_report_exit_on_leaks();
}

test byte_array_tests[] = {
    {"should_successfully_create_byte_array_with_data", should_successfully_create_byte_array_with_data},
    {"should_succeed_if_data_length_is_0", should_succeed_if_data_length_is_0},
    {"should_successfully_create_empty_byte_array_with_data", should_successfully_create_empty_byte_array_with_data},
    {"should_succeed_to_create_stream_with_some_capacity", should_succeed_to_create_stream_with_some_capacity},
    {"should_successfully_append_a_single_subset_of_bytes", should_successfully_append_a_single_subset_of_bytes},
    {"should_successfully_append_a_multiple_subset_of_bytes", should_successfully_append_a_multiple_subset_of_bytes},
    {
        "should_successfully_append_a_large_subset_of_bytes_and_resize",
        should_successfully_append_a_large_subset_of_bytes_and_resize
    },
    {"should_successfully_copy_subset", should_successfully_copy_subset},
    {"should_successfully_compare_byte_arrays1", should_successfully_compare_byte_arrays1},
    {"should_successfully_compare_byte_arrays2", should_successfully_compare_byte_arrays2},
    {"should_successfully_compare_byte_arrays3", should_successfully_compare_byte_arrays3},
    {"should_successfully_compare_byte_array_to_raw_data", should_successfully_compare_byte_array_to_raw_data},
    {"should_fail_compare_byte_array_to_raw_data", should_fail_compare_byte_array_to_raw_data},
    {"should_fail_compare_byte_arrays1", should_fail_compare_byte_arrays1},
    {"should_fail_compare_byte_arrays2", should_fail_compare_byte_arrays2},
    {"should_successfully_append_singe_byte", should_successfully_append_singe_byte},
    {"should_successfully_append_raw_data", should_successfully_append_raw_data},
    {"should_successfully_reversed_data", should_successfully_reversed_data},
    {"should_successfully_left_shift_bits1", should_successfully_left_shift_bits1},
    {"should_successfully_left_shift_bits2", should_successfully_left_shift_bits2},
    {"should_successfully_xor_bits", should_successfully_xor_bits},
    {"should_successfully_create_byte_array_from_hex", should_successfully_create_byte_array_from_hex},
    {"should_successfully_create_hex_from_byte_array", should_successfully_create_hex_from_byte_array},

#ifdef _WIN32
    {"should_successfully_dump_data", should_successfully_dump_data},
#endif

    TEST_END
};
