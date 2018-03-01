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

#include "list_tests.h"

#include <enjector/core/list.h>
#include <enjector/core/xmemory.h>

#include <string.h>
#include <stdio.h>

static void should_successfully_create_list_with_untyped_items() {
    list* l = list_create();
    list_item* item;

    TEST_ASSERT_PTR_NOT_NULL(l);

    list_add(l, (void*) "Hello1");
    list_add(l, (void*) "Hello2");

    TEST_ASSERT_EQUAL_INT(list_count(l), 2);

    for(int i = 0; i < list_count(l); i++) {
        item = list_get_item(l, i);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(item);
        TEST_ASSERT_PTR_NULL(item->type);
        TEST_ASSERT_PTR_NOT_NULL(item->value);
    }

    list_free(l);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_list_with_untyped_items_using_enumerable() {
    list* l = list_create();
    list_item* item;

    TEST_ASSERT_PTR_NOT_NULL(l);

    list_add(l, (void*) "Hello1");
    list_add(l, (void*) "Hello2");

    list_item** items = list_enumerable(l);
    TEST_ASSERT_EQUAL_INT(list_count(l), 2);

    for(int i = 0; i < list_count(l); i++) {
        item = items[i];
        TEST_ASSERT_PTR_NOT_NULL_FATAL(item);
        TEST_ASSERT_PTR_NULL(item->type);
        TEST_ASSERT_PTR_NOT_NULL(item->value);
    }

    list_free(l);

    xmemory_report_exit_on_leaks();
}

typedef struct mytest_t {
    int a;
    int b;
} mytest;

static void should_successfully_create_list_with_typed_items() {
    list* l = list_create();
    mytest test_struct;
    const char* test_string = "Hello";
    list_item* item;

    TEST_ASSERT_PTR_NOT_NULL(l);

    list_add_with_type(l, "struct", &test_struct);
    list_add_with_type(l, "string", (void*)test_string);

    TEST_ASSERT_EQUAL_INT(list_count(l), 2);

    int struct_type_count = 0;
    int string_type_count = 0;

    for(int i = 0; i < list_count(l); i++) {
        item = list_get_item(l, i);
        TEST_ASSERT_PTR_NOT_NULL_FATAL(item);
        TEST_ASSERT_PTR_NOT_NULL(item->type);
        TEST_ASSERT_PTR_NOT_NULL(item->value);

        if(!strcmp(item->type, "struct")) {
            struct_type_count++;
        } else if(!strcmp(item->type, "string")) {
            string_type_count++;
        }
    }

    TEST_ASSERT_EQUAL_INT(struct_type_count, 1);
    TEST_ASSERT_EQUAL_INT(string_type_count, 1);

    list_free(l);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_clear_a_populated_list() {
    list* l = list_create();
    mytest test_struct;
    const char* test_string = "Hello";
    int struct_type_count;
    int string_type_count;
    list_item* item;

    TEST_ASSERT_PTR_NOT_NULL(l);

    list_add_with_type(l, "struct", &test_struct);
    list_add_with_type(l, "string", (void*)test_string);

    TEST_ASSERT_EQUAL_INT(list_count(l), 2);

    list_clear(l);

    TEST_ASSERT_EQUAL_INT(list_count(l), 0);

    list_free(l);

    xmemory_report_exit_on_leaks();
}

typedef struct customer_t {
    int id;
    char* name;
} customer;

static void should_successfully_create_list_and_add_items() {
    list(customer, customers);

    list_add_new(customers, a, {
        a->id = 1;
        a->name = "fred1";
    });

    list_add_new(customers, b, {
        b->id = 2;
        b->name = "fred2";
    });

    TEST_ASSERT_EQUAL_INT(2, list_count(customers));

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_list_and_add_items_then_iterate() {
    list* l = list_create();

    TEST_ASSERT_PTR_NOT_NULL(l);

    list_add(l, "test1");
    list_add(l, "test2");

    TEST_ASSERT_EQUAL_INT(2, list_count(l));

    int i = 0;
    list_foreach_of_begin(l, char*, value) {
        switch(i++) {
        case 0:
            TEST_ASSERT_TRUE(!strcmp(value, "test1"));
            break;

        case 1:
            TEST_ASSERT_TRUE(!strcmp(value, "test2"));
            break;

        default:
            TEST_ASSERT_FALSE(true);
            break;
        }
    } list_foreach_of_end

    list_free(l);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_iterate_over_list_using_foreach() {
    // Prepare
    customer expected[2] = {
        { 1, "customer1" },
        { 2, "customer2" },
    };

    list(customer, customers);

    for(int i = 0; i < 2; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    // Act
    int j = 0;

    list_foreach_begin(customers, c) {
        TEST_ASSERT_EQUAL_INT(expected[j].id, c->id);
        TEST_ASSERT_TRUE(!strcmp(expected[j].name, c->name));
        j++;
    } list_foreach_of_end

    TEST_ASSERT_EQUAL_INT(2, j);

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}


static void should_successfully_filter_list() {
    list(customer, customers);

    list_add_new(customers, a, {
        a->id = 1;
        a->name = "fred1";
    });

    list_add_new(customers, b, {
        b->id = 2;
        b->name = "fred2";
    });

    list_add_new(customers, b, {
        b->id = 3;
        b->name = "fred3";
    });

    list_add_new(customers, b, {
        b->id = 4;
        b->name = "fred4";
    });

    list(customer, filtered_customers);
    list_filter(customers, c, filtered_customers, !strcmp(c->name, "fred2") || !strcmp(c->name, "fred3"));

    TEST_ASSERT_EQUAL_INT(2, list_count(filtered_customers));

    TEST_ASSERT_EQUAL_INT(2, list_get(filtered_customers, 0)->id);
    TEST_ASSERT_TRUE(!strcmp("fred2", list_get(filtered_customers, 0)->name));

    TEST_ASSERT_EQUAL_INT(3, list_get(filtered_customers, 1)->id);
    TEST_ASSERT_TRUE(!strcmp("fred3", list_get(filtered_customers, 1)->name));

    list_dispose(customers);
    list_free(filtered_customers);

    xmemory_report_exit_on_leaks();
}

typedef struct order_t {
    int id;
    char* product;
    char* customer;
    bool delivered;
} order;

typedef struct delivery_label_t {
    char label[1024];
} delivery_label;

static void should_successfully_map_from_one_list_to_another() {
    list(order, orders);

    list_add_new(orders, a, {
        a->id = 1;
        a->product = "watch1";
        a->customer = "fred1";
        a->delivered = true;
    });

    list_add_new(orders, a, {
        a->id = 2;
        a->product = "watch2";
        a->customer = "fred2";
        a->delivered = false;
    });

    list(delivery_label, delivery_labels);

    list_map_begin(orders, o, delivery_labels, l) {
        snprintf(l->label,
                 sizeof(l->label),
                 "Order: %d. Product: %s. Customer: %s", o->id, o->product, o->customer);
    } list_map_end;

    TEST_ASSERT_EQUAL_INT(2, list_count(delivery_labels));
    TEST_ASSERT_TRUE(!strcmp("Order: 1. Product: watch1. Customer: fred1", list_get(delivery_labels, 0)->label));
    TEST_ASSERT_TRUE(!strcmp("Order: 2. Product: watch2. Customer: fred2", list_get(delivery_labels, 1)->label));

    list_dispose(orders);
    list_dispose(delivery_labels);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_get_first_and_last_item_from_list() {
    customer expected[4] = {
        { 1, "customer1" },
        { 2, "customer2" },
        { 3, "customer3" },
        { 4, "customer4" },
    };

    list(customer, customers);

    for(int i = 0; i < 4; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    list_first(customers, first_customer);
    TEST_ASSERT_EQUAL_INT(expected[0].id, first_customer->id);
    TEST_ASSERT_TRUE(!strcmp(expected[0].name, first_customer->name));

    list_last(customers, last_customer);
    TEST_ASSERT_EQUAL_INT(expected[3].id, last_customer->id);
    TEST_ASSERT_TRUE(!strcmp(expected[3].name, last_customer->name));

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_take_first_few_items() {
    order expected[4] = {
        { 1, "watch1", "customer1", true },
        { 2, "watch2", "customer2", false },
        { 3, "watch3", "customer3", false },
        { 4, "watch4", "customer4", true },
    };

    list(order, orders);

    for(int i = 0; i < 4; i++) {
        list_add_new(orders, a, {
            a->id = expected[i].id;
            a->product = expected[i].product;
            a->customer = expected[i].customer;
            a->delivered = expected[i].delivered;
        });
    }

    list(order, taken_orders);
    list_take(orders, 2, taken_orders);

    TEST_ASSERT_EQUAL_INT(2, list_count(taken_orders));
    TEST_ASSERT_EQUAL_INT(expected[0].id, list_get(taken_orders, 0)->id);
    TEST_ASSERT_TRUE(!strcmp(expected[0].product, list_get(taken_orders, 0)->product));
    TEST_ASSERT_TRUE(!strcmp(expected[0].customer, list_get(taken_orders, 0)->customer));
    TEST_ASSERT_EQUAL_BOOL(expected[0].delivered, list_get(taken_orders, 0)->delivered);

    TEST_ASSERT_EQUAL_INT(expected[1].id, list_get(taken_orders, 1)->id);
    TEST_ASSERT_TRUE(!strcmp(expected[1].product, list_get(taken_orders, 1)->product));
    TEST_ASSERT_TRUE(!strcmp(expected[1].customer, list_get(taken_orders, 1)->customer));
    TEST_ASSERT_EQUAL_BOOL(expected[1].delivered, list_get(taken_orders, 1)->delivered);

    list_dispose(orders);
    list_free(taken_orders);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_attempt_to_take_more_items_than_in_list() {
    order expected[4] = {
        { 1, "watch1", "customer1", true },
        { 2, "watch2", "customer2", false },
        { 3, "watch3", "customer3", false },
        { 4, "watch4", "customer4", true },
    };

    list(order, orders);

    for(int i = 0; i < 4; i++) {
        list_add_new(orders, a, {
            a->id = expected[i].id;
            a->product = expected[i].product;
            a->customer = expected[i].customer;
            a->delivered = expected[i].delivered;
        });
    }

    list(order, taken_orders);
    list_take(orders, 4, taken_orders);

    TEST_ASSERT_EQUAL_INT(4, list_count(taken_orders));
    TEST_ASSERT_EQUAL_INT(expected[0].id, list_get(taken_orders, 0)->id);
    TEST_ASSERT_EQUAL_INT(expected[1].id, list_get(taken_orders, 1)->id);
    TEST_ASSERT_EQUAL_INT(expected[2].id, list_get(taken_orders, 2)->id);
    TEST_ASSERT_EQUAL_INT(expected[3].id, list_get(taken_orders, 3)->id);

    list_dispose(orders);
    list_free(taken_orders);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_take_last_few_items() {
    order expected[4] = {
        { 1, "watch1", "customer1", true },
        { 2, "watch2", "customer2", false },
        { 3, "watch3", "customer3", false },
        { 4, "watch4", "customer4", true },
    };

    list(order, orders);

    for(int i = 0; i < 4; i++) {
        list_add_new(orders, a, {
            a->id = expected[i].id;
            a->product = expected[i].product;
            a->customer = expected[i].customer;
            a->delivered = expected[i].delivered;
        });
    }

    list(order, taken_orders);
    list_take_right(orders, 2, taken_orders);

    TEST_ASSERT_EQUAL_INT(2, list_count(taken_orders));
    TEST_ASSERT_EQUAL_INT(expected[3].id, list_get(taken_orders, 0)->id);
    TEST_ASSERT_TRUE(!strcmp(expected[3].product, list_get(taken_orders, 0)->product));
    TEST_ASSERT_TRUE(!strcmp(expected[3].customer, list_get(taken_orders, 0)->customer));
    TEST_ASSERT_EQUAL_BOOL(expected[3].delivered, list_get(taken_orders, 0)->delivered);

    TEST_ASSERT_EQUAL_INT(expected[2].id, list_get(taken_orders, 1)->id);
    TEST_ASSERT_TRUE(!strcmp(expected[2].product, list_get(taken_orders, 1)->product));
    TEST_ASSERT_TRUE(!strcmp(expected[2].customer, list_get(taken_orders, 1)->customer));
    TEST_ASSERT_EQUAL_BOOL(expected[2].delivered, list_get(taken_orders, 1)->delivered);

    list_dispose(orders);
    list_free(taken_orders);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_attempt_to_take_last_more_items_than_in_list() {
    order expected[4] = {
        { 1, "watch1", "customer1", true },
        { 2, "watch2", "customer2", false },
        { 3, "watch3", "customer3", false },
        { 4, "watch4", "customer4", true },
    };

    list(order, orders);

    for(int i = 0; i < 4; i++) {
        list_add_new(orders, a, {
            a->id = expected[i].id;
            a->product = expected[i].product;
            a->customer = expected[i].customer;
            a->delivered = expected[i].delivered;
        });
    }

    list(order, taken_orders);
    list_take_right(orders, 20, taken_orders);

    TEST_ASSERT_EQUAL_INT(4, list_count(taken_orders));
    TEST_ASSERT_EQUAL_INT(expected[3].id, list_get(taken_orders, 0)->id);
    TEST_ASSERT_EQUAL_INT(expected[2].id, list_get(taken_orders, 1)->id);
    TEST_ASSERT_EQUAL_INT(expected[1].id, list_get(taken_orders, 2)->id);
    TEST_ASSERT_EQUAL_INT(expected[0].id, list_get(taken_orders, 3)->id);

    list_dispose(orders);
    list_free(taken_orders);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_item_by_ref_from_list() {
    // Prepare
    customer expected[3] = {
        { 1, "customer1" },
        { 2, "customer2" },
        { 3, "customer3" },
    };

    list(customer, customers);

    for(int i = 0; i < 3; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    for(int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i].id, ((customer*)customers->data[i]->value)->id);
    }

    // Act
    // - Remove middle item
    list_item* item = list_get_item(customers, 1);
    TEST_ASSERT_EQUAL_INT(expected[1].id, ((customer*)item->value)->id);

    xmemory_free(item->value);
    list_remove_item(customers, item);

    TEST_ASSERT_EQUAL_INT(2, list_count(customers));

    item = list_get_item(customers, 0);
    TEST_ASSERT_EQUAL_INT(expected[0].id, ((customer*)item->value)->id);

    item = list_get_item(customers, 1);
    TEST_ASSERT_EQUAL_INT(expected[2].id, ((customer*)item->value)->id);

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_item_by_index_from_list() {
    // Prepare
    customer expected[3] = {
        { 1, "customer1" },
        { 2, "customer2" },
        { 3, "customer3" },
    };

    list(customer, customers);

    for(int i = 0; i < 3; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    for(int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i].id, ((customer*)customers->data[i]->value)->id);
    }

    // Act
    // - Remove middle item
    list_dispose_item_at(customers, 1);

    TEST_ASSERT_EQUAL_INT(2, list_count(customers));

    list_item* item = list_get_item(customers, 0);
    TEST_ASSERT_EQUAL_INT(expected[0].id, ((customer*)item->value)->id);

    item = list_get_item(customers, 1);
    TEST_ASSERT_EQUAL_INT(expected[2].id, ((customer*)item->value)->id);

    list_dispose(customers);
    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_item_by_index_from_begining_of_list() {
    // Prepare
    customer expected[3] = {
        { 1, "customer1" },
        { 2, "customer2" },
        { 3, "customer3" },
    };

    list(customer, customers);

    for(int i = 0; i < 3; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    for(int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i].id, ((customer*)customers->data[i]->value)->id);
    }

    // Act
    // - Remove middle item
    list_dispose_item_at(customers, 0);

    TEST_ASSERT_EQUAL_INT(2, list_count(customers));

    list_item* item = list_get_item(customers, 0);
    TEST_ASSERT_EQUAL_INT(expected[1].id, ((customer*)item->value)->id);

    item = list_get_item(customers, 1);
    TEST_ASSERT_EQUAL_INT(expected[2].id, ((customer*)item->value)->id);

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_item_by_index_from_end_of_list() {
    // Prepare
    customer expected[3] = {
        { 1, "customer1" },
        { 2, "customer2" },
        { 3, "customer3" },
    };

    list(customer, customers);

    for(int i = 0; i < 3; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    for(int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i].id, ((customer*)customers->data[i]->value)->id);
    }

    // Act
    // - Remove middle item
    list_dispose_item_at(customers, 2);

    TEST_ASSERT_EQUAL_INT(2, list_count(customers));

    list_item* item = list_get_item(customers, 0);
    TEST_ASSERT_EQUAL_INT(expected[0].id, ((customer*)item->value)->id);

    item = list_get_item(customers, 1);
    TEST_ASSERT_EQUAL_INT(expected[1].id, ((customer*)item->value)->id);

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_item_by_value_from_list() {
    // Prepare
    customer expected[3] = {
        { 1, "customer1" },
        { 2, "customer2" },
        { 3, "customer3" },
    };

    list(customer, customers);

    for(int i = 0; i < 3; i++) {
        list_add_new(customers, a, {
            a->id = expected[i].id;
            a->name = expected[i].name;
        });
    }

    for(int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i].id, ((customer*)customers->data[i]->value)->id);
    }

    // Act
    // - Remove middle item
    list_item* item = list_get_item(customers, 1);
    customer* c = (customer*)item->value;
    TEST_ASSERT_EQUAL_INT(expected[1].id, c->id);

    TEST_ASSERT_TRUE(list_remove_item_by_value(customers, c));
    xmemory_free(c);

    TEST_ASSERT_EQUAL_INT(2, list_count(customers));

    item = list_get_item(customers, 0);
    TEST_ASSERT_EQUAL_INT(expected[0].id, ((customer*)item->value)->id);

    item = list_get_item(customers, 1);
    TEST_ASSERT_EQUAL_INT(expected[2].id, ((customer*)item->value)->id);

    list_dispose(customers);

    xmemory_report_exit_on_leaks();
}

test list_tests[] = {
    { "should_successfully_create_list_with_untyped_items", should_successfully_create_list_with_untyped_items },
    { "should_successfully_create_list_with_untyped_items_using_enumerable", should_successfully_create_list_with_untyped_items_using_enumerable },
    { "should_successfully_create_list_with_typed_items", should_successfully_create_list_with_typed_items },
    { "should_successfully_clear_a_populated_list", should_successfully_clear_a_populated_list },
    { "should_successfully_create_list_and_add_items_then_iterate", should_successfully_create_list_and_add_items_then_iterate },
    { "should_successfully_create_list_and_add_items", should_successfully_create_list_and_add_items },
    { "should_successfully_iterate_over_list_using_foreach", should_successfully_iterate_over_list_using_foreach },
    { "should_successfully_filter_list", should_successfully_filter_list},
    { "should_successfully_map_from_one_list_to_another", should_successfully_map_from_one_list_to_another },
    { "should_successfully_get_first_and_last_item_from_list", should_successfully_get_first_and_last_item_from_list },
    { "should_successfully_take_first_few_items", should_successfully_take_first_few_items},
    { "should_successfully_attempt_to_take_more_items_than_in_list", should_successfully_attempt_to_take_more_items_than_in_list},
    { "should_successfully_take_last_few_items", should_successfully_take_last_few_items},
    { "should_successfully_attempt_to_take_last_more_items_than_in_list", should_successfully_attempt_to_take_last_more_items_than_in_list},
    { "should_successfully_remove_item_by_ref_from_list", should_successfully_remove_item_by_ref_from_list },
    { "should_successfully_remove_item_by_index_from_list", should_successfully_remove_item_by_index_from_list },
    { "should_successfully_remove_item_by_index_from_begining_of_list", should_successfully_remove_item_by_index_from_begining_of_list },
    { "should_successfully_remove_item_by_index_from_end_of_list", should_successfully_remove_item_by_index_from_end_of_list },
    { "should_successfully_remove_item_by_value_from_list", should_successfully_remove_item_by_value_from_list },
    TEST_END
};
