/*
* list_tests.c
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

#include "map_tests.h"

#include <enjector/core/map.h>
#include <enjector/core/xmemory.h>

#include <string.h>
#include <stdio.h>
#include <assert.h>

static void should_successfully_create_map_with_items() {
    map* m = map_create();

    assert(m);

    TEST_ASSERT_TRUE(map_set(m, "Key1", (void*) "Hello1"));
    TEST_ASSERT_TRUE(map_set(m, "Key2", (void*) "Hello2"));

    TEST_ASSERT_EQUAL_INT(map_count(m), 2);
    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key1")->value, "Hello1"));
    TEST_ASSERT_PTR_NULL(map_get_item(m, "Key1")->type, NULL);

    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key2")->value, "Hello2"));
    TEST_ASSERT_PTR_NULL(map_get_item(m, "Key2")->type);

    map_item ** items = map_enumerable(m);

    for(unsigned int i = 0; i < map_count(m); i++) {
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]->name);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]->value);
    }

    map_free(m);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_map_with_typed_items() {
    map_item** items;
    unsigned int i;
    map* m = map_create();

    assert(m);

    TEST_ASSERT_TRUE(map_set_with_type(m, "Key1", "string", (void*) "Hello1"));
    TEST_ASSERT_TRUE(map_set_with_type(m, "Key2", "string", (void*) "Hello2"));

    TEST_ASSERT_EQUAL_INT(map_count(m), 2);
    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key1")->value, "Hello1"));
    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key1")->type, "string"));

    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key2")->value, "Hello2"));
    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key2")->type, "string"));

    items = map_enumerable(m);

    for(i = 0; i < map_count(m); i++) {
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]->name);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]->type);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(items[i]->value);
    }

    map_free(m);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_update_existing_item_in_map() {
    map* m = map_create();

    assert(m);

    TEST_ASSERT_TRUE(map_set(m, "Key1", (void*) "Hello1"));
    TEST_ASSERT_TRUE(map_set(m, "Key1", (void*) "Test"));

    TEST_ASSERT_EQUAL_INT(map_count(m), 1);
    TEST_ASSERT_TRUE(!strcmp((const char*)map_get_item(m, "Key1")->value, "Test"));

    map_free(m);

    xmemory_report_exit_on_leaks();
}

typedef struct customer_t {
    int id;
    char* name;
} customer;

static void should_successfully_iterate_over_map_using_foreach() {
    // Prepare
    map(customer, customers)
    customer expected[2] = {
        { 1, "customer1" },
        { 2, "customer2" },
    };

    for(int i = 0; i < 2; i++) {
        char key[10]; snprintf(key, 10, "CUS%d", expected[i].id);
        map_set_new(customers, key, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    // Act
    int j = 0;

    map_foreach_begin(customers, actual_key, c) {
        char expected_key[10]; snprintf(expected_key, 10, "CUS%d", expected[i].id);
        TEST_ASSERT_EQUAL_STRING(expected_key, actual_key);
        TEST_ASSERT_EQUAL_INT(expected[j].id, c->id);
        TEST_ASSERT_EQUAL_STRING(expected[j].name, c->name);
        j++;
    } map_foreach_end;

    map_dispose(customers);
    xmemory_report_exit_on_leaks();
}

static void should_successfully_filter_map() {
    map(customer, customers);

    map_set_new(customers, "ORD1", a, {
        a->id = 1;
        a->name = "fred1";
    });

    map_set_new(customers, "ORD2", b, {
        b->id = 2;
        b->name = "fred2";
    });

    map_set_new(customers, "ORD3", b, {
        b->id = 3;
        b->name = "fred3";
    });

    map_set_new(customers, "ORD4", b, {
        b->id = 4;
        b->name = "fred4";
    });

    map(customer, filtered_customers);
    map_filter(customers, key, c, filtered_customers, !strcmp(c->name, "fred2") || !strcmp(c->name, "fred3"));

    TEST_ASSERT_EQUAL_INT(2, map_count(filtered_customers));
    TEST_ASSERT_TRUE(map_exists(filtered_customers, "ORD2"));
    TEST_ASSERT_TRUE(map_exists(filtered_customers, "ORD3"));

    customer* c2 = map_get(filtered_customers, "ORD2");
    TEST_ASSERT_EQUAL_INT(2, c2->id);
    TEST_ASSERT_EQUAL_STRING("fred2", c2->name);

    customer* c3 = map_get(filtered_customers, "ORD3");
    TEST_ASSERT_EQUAL_INT(3, c3->id);
    TEST_ASSERT_EQUAL_STRING("fred3", c3->name);

    map_dispose(customers);
    map_free(filtered_customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_clear_map() {
    map(customer, customers);

    map_set_new(customers, "ORD1", a, {
        a->id = 1;
        a->name = "fred1";
    });

    map_set_new(customers, "ORD2", b, {
        b->id = 2;
        b->name = "fred2";
    });

    map_set_new(customers, "ORD3", b, {
        b->id = 3;
        b->name = "fred3";
    });

    map_set_new(customers, "ORD4", b, {
        b->id = 4;
        b->name = "fred4";
    });

    TEST_ASSERT_EQUAL_INT(4, map_count(customers));
    map_clear_dispose(customers);
    TEST_ASSERT_EQUAL_INT(0, map_count(customers));

    map_free(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_from_map() {
    map(customer, customers);

    map_set_new(customers, "ORD1", a, {
        a->id = 1;
        a->name = "fred1";
    });

    map_set_new(customers, "ORD2", b, {
        b->id = 2;
        b->name = "fred2";
    });

    map_set_new(customers, "ORD3", b, {
        b->id = 3;
        b->name = "fred3";
    });

    map_set_new(customers, "ORD4", b, {
        b->id = 4;
        b->name = "fred4";
    });

    map_item_dispose(customers, "ORD2");

    TEST_ASSERT_EQUAL_INT(3, map_count(customers));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD1"));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD3"));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD4"));

    TEST_ASSERT_FALSE(map_exists(customers, "ORD2"));

    map_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_from_begining_of_map() {
    map(customer, customers);

    map_set_new(customers, "ORD1", a, {
        a->id = 1;
        a->name = "fred1";
    });

    map_set_new(customers, "ORD2", b, {
        b->id = 2;
        b->name = "fred2";
    });

    map_set_new(customers, "ORD3", b, {
        b->id = 3;
        b->name = "fred3";
    });

    map_set_new(customers, "ORD4", b, {
        b->id = 4;
        b->name = "fred4";
    });

    map_item_dispose(customers, "ORD1");

    TEST_ASSERT_EQUAL_INT(3, map_count(customers));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD2"));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD3"));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD4"));

    TEST_ASSERT_FALSE(map_exists(customers, "ORD1"));

    map_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_from_end_of_map() {
    map(customer, customers);

    map_set_new(customers, "ORD1", a, {
        a->id = 1;
        a->name = "fred1";
    });

    map_set_new(customers, "ORD2", b, {
        b->id = 2;
        b->name = "fred2";
    });

    map_set_new(customers, "ORD3", b, {
        b->id = 3;
        b->name = "fred3";
    });

    map_set_new(customers, "ORD4", b, {
        b->id = 4;
        b->name = "fred4";
    });

    map_item_dispose(customers, "ORD4");

    TEST_ASSERT_EQUAL_INT(3, map_count(customers));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD1"));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD2"));
    TEST_ASSERT_TRUE(map_exists(customers, "ORD3"));

    TEST_ASSERT_FALSE(map_exists(customers, "ORD4"));

    map_dispose(customers);

    xmemory_report_exit_on_leaks();
}

test map_tests[] = {
    { "should_successfully_create_map_with_items", should_successfully_create_map_with_items },
    { "should_successfully_create_map_with_typed_items", should_successfully_create_map_with_typed_items },
    { "should_successfully_update_existing_item_in_map", should_successfully_update_existing_item_in_map },
    { "should_successfully_iterate_over_map_using_foreach", should_successfully_iterate_over_map_using_foreach },
    { "should_successfully_filter_map", should_successfully_filter_map },
    { "should_successfully_clear_map", should_successfully_clear_map },
    { "should_successfully_remove_from_map", should_successfully_remove_from_map },
    { "should_successfully_remove_from_begining_of_map", should_successfully_remove_from_begining_of_map },
    { "should_successfully_remove_from_end_of_map", should_successfully_remove_from_end_of_map },
    TEST_END
};
