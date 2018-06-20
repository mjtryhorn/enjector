/*
* hex_tests.c
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

#include "hex_tests.h"

#include <enjector/core/hex.h>
#include <enjector/core/xmemory.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void should_fail_to_convert_bin_to_hex_on_empty_bin1() {
    char* response = NULL;
    unsigned char* bin = (unsigned char*) "";

    response = hex_from_bin(bin, 0);
    TEST_ASSERT_PTR_NULL(response);

    xmemory_report_exit_on_leaks();
}

static void should_fail_to_convert_bin_to_hex_on_empty_bin2() {
    unsigned char* bin = 0;

    char* response = hex_from_bin(bin, 0);
    TEST_ASSERT_PTR_NULL(response);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_bin_to_hex1() {
    unsigned char bin[] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f };

    char* response = hex_from_bin(bin, 16);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(response);
    TEST_ASSERT_TRUE(!strcmp("404142434445464748494a4b4c4d4e4f", response));

    hex_free(response);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_bin_to_hex2() {
    unsigned char bin[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    char* response = hex_from_bin(bin, 8);

    TEST_ASSERT_PTR_NOT_NULL_FATAL(response);
    TEST_ASSERT_TRUE(!strcmp("0000000000000000", response));

    hex_free(response);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_hex_to_bin1() {
    const char* hex = "404142";
    const size_t hex_len = strlen(hex);

    unsigned char* bin = hex_to_bin(hex, hex_len);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(bin);

    char* bin_hex = hex_from_bin(bin, hex_len / 2);
    TEST_ASSERT_TRUE(!strcmp(bin_hex, hex));

    hex_free(bin_hex);
    hex_free(bin);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_hex_to_bin2() {
    const char* hex = "48656c6c6f20576f726c642031323334";
    const size_t hex_len = strlen(hex);

    unsigned char* bin = hex_to_bin(hex, hex_len);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(bin);

    char* bin_hex = hex_from_bin(bin, hex_len / 2);
    TEST_ASSERT_TRUE(!strcmp(bin_hex, hex));

    hex_free(bin_hex);
    hex_free(bin);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_hex_to_unsigned_integer1() {
    const int expected = 25;
    const char* hex = "19";

    const size_t actual = hex_to_unsigned_integer(hex);
    TEST_ASSERT_EQUAL_SIZE(expected, actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_hex_to_unsigned_integer2() {
    const int expected = 680;
    const char* hex = "2a8";

    const size_t actual = hex_to_unsigned_integer(hex);
    TEST_ASSERT_EQUAL_SIZE(expected, actual);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_convert_hex_to_unsigned_integer3() {
    const int expected = 2658295;
    const char* hex = "288FF7";

    const size_t actual = hex_to_unsigned_integer(hex);
    TEST_ASSERT_EQUAL_SIZE(expected, actual);

    xmemory_report_exit_on_leaks();
}

test hex_tests[] = {
    { "should_fail_to_convert_bin_to_hex_on_empty_bin1", should_fail_to_convert_bin_to_hex_on_empty_bin1 },
    { "should_fail_to_convert_bin_to_hex_on_empty_bin2", should_fail_to_convert_bin_to_hex_on_empty_bin2 },
    { "should_successfully_convert_bin_to_hex1", should_successfully_convert_bin_to_hex1 },
    { "should_successfully_convert_bin_to_hex2", should_successfully_convert_bin_to_hex2 },
    { "should_successfully_convert_hex_to_bin1", should_successfully_convert_hex_to_bin1 },
    { "should_successfully_convert_hex_to_bin2", should_successfully_convert_hex_to_bin2 },
    { "should_successfully_convert_hex_to_unsigned_integer1", should_successfully_convert_hex_to_unsigned_integer1 },
    { "should_successfully_convert_hex_to_unsigned_integer2", should_successfully_convert_hex_to_unsigned_integer2 },
    { "should_successfully_convert_hex_to_unsigned_integer3", should_successfully_convert_hex_to_unsigned_integer3 },
    TEST_END
};
