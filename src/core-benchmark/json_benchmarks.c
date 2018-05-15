/*
* json_benchmarks.c
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
#include <enjector/core/benchmark.h>
#include <enjector/core/test.h>

#include <stdio.h>
#include <limits.h>

//#define CHECK

typedef struct message_t {
    bool is_reply;
    char* from;
    char* to;
    char* subject;
    char* body;
} message;

serialisable(message, {
    as_bool(is_reply);
    as_string(from);
    as_string(to);
    as_string(subject);
    as_string(body);
});

static int json_serialise_simple(int argc, const char * argv[]) {

    int count = 50000;
    int n = count;

    message* m = xmemory_new(message);

    m->is_reply = true;
    m->from = text_clone("Fred Smith");
    m->to = text_clone("Bob Jones");
    m->subject = text_clone("Hello");
    m->body = text_clone("How are you doing?");

    while (n-- > 0) {
        string_buffer* json = message_serialise(m);

#ifdef CHECK
        TEST_ASSERT_TRUE(text_equals(json->data, "{\"is_reply\":true,\"from\":\"Fred Smith\",\"to\":\"Bob Jones\",\"subject\":\"Hello\",\"body\":\"How are you doing?\"}"));
#endif


        string_buffer_free(json);
    }

    message_dispose(m);

#ifdef CHECK
    xmemory_report_exit_on_leaks();
#endif

    return count;
}

static int json_deserialise_simple(int argc, const char * argv[]) {

    int count = 50000;
    int n = count;

    while (n-- > 0) {

        char* json = "{\"is_reply\":true,\"from\":\"Fred Smith\",\"to\":\"Bob Jones\",\"subject\":\"Hello\",\"body\":\"How are you doing?\"}";

        message* p = message_deserialise(json);

        //	#ifdef CHECK
        TEST_ASSERT_PTR_NOT_NULL_FATAL(p);

        TEST_ASSERT_TRUE(p->is_reply);
        TEST_ASSERT_TRUE(text_equals(p->from, "Fred Smith"));
        TEST_ASSERT_TRUE(text_equals(p->to, "Bob Jones"));
        TEST_ASSERT_TRUE(text_equals(p->subject, "Hello"));
        TEST_ASSERT_TRUE(text_equals(p->body, "How are you doing?"));

        //	#endif

        message_dispose(p);
    }

#ifdef CHECK
    xmemory_report_exit_on_leaks();
#endif

    return count;
}

typedef struct security_t {
    char* type;
    char* name;
} security;

serialisable(security, {
    as_string(type);
    as_string(name);
});

typedef struct address_t {
    bool is_billing;
    char* street1;
    char* street2;
    security* security;
} address;

serialisable(address, {
    as_bool(is_billing);
    as_string(street1);
    as_string(street2);
    as_object(security, security);
});

typedef struct person_t {
    bool enabled;
    char* firstname;
    char* lastname;
    int age;
    long created_on;

    address* home;
    list* addresses;
} person;

serialisable(person, {
    as_bool(enabled);
    as_string(firstname);
    as_string(lastname);
    as_integer(age);
    as_long(created_on);

    as_object(address, home);
    as_list(address, addresses);
});

static int json_serialise_complex(int argc, const char * argv[]) {

    int count = 50000;
    int n = count;

    person* p = xmemory_new(person);

    p->enabled = true;
    p->firstname = text_clone("Fred");
    p->lastname = text_clone("Smith");
    p->created_on = LONG_MAX;
    p->age = 42;

    p->home = xmemory_new(address);
    p->home->is_billing = true;
    p->home->street1 = text_clone("street 1");
    p->home->street2 = text_clone("street 2");
    p->home->security = xmemory_new(security);
    p->home->security->type = text_clone("silent");
    p->home->security->name = text_clone("Vanguard");

    p->addresses = list_create();

    address* add1 = xmemory_new(address);
    add1->is_billing = true;
    add1->street1 = text_clone("A list street 1");
    add1->street2 = text_clone("A list street 2");
    add1->security = xmemory_new(security);
    add1->security->type = text_clone("A Alarm Type");
    add1->security->name = text_clone("A Alarm Name");
    list_add(p->addresses, add1);

    address *add2 = xmemory_new(address);
    add2->is_billing = false;
    add2->street1 = text_clone("B list street 1");
    add2->street2 = text_clone("B list street 2");
    add2->security = xmemory_new(security);
    add2->security->type = text_clone("B Alarm Type");
    add2->security->name = text_clone("B Alarm Name");
    list_add(p->addresses, add2);

    while (n-- > 0) {

        string_buffer* json = person_serialise(p);

#ifdef CHECK
        TEST_ASSERT_TRUE(text_equals(json->data, "{\"enabled\":true,\"firstname\":\"Fred\",\"lastname\":\"Smith\",\"age\":42,\"created_on\":2147483647,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":[{\"is_billing\":true,\"street1\":\"A list street 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":false,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}]}"));
#endif

        string_buffer_free(json);
    }

    person_dispose(p);

#ifdef CHECK
    xmemory_report_exit_on_leaks();
#endif

    return count;
}

static int json_deserialise_complex(int argc, const char * argv[]) {

    int count = 50000;
    int n = count;

    while (n-- > 0) {

        char* json = "{\"firstname\":\"Fred\",\"lastname\":\"Smith\",\"age\":42,\"created_on\":2147483647,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":[{\"street1\":\"A list street 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":true,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}]}";

        person* p = person_deserialise(json);

#ifdef CHECK
        TEST_ASSERT_PTR_NOT_NULL_FATAL(p);

        TEST_ASSERT_TRUE(text_equals(p->firstname, "Fred"));
        TEST_ASSERT_TRUE(text_equals(p->lastname, "Smith"));
        TEST_ASSERT_EQUAL(p->age, 42);
        TEST_ASSERT_EQUAL(p->created_on, LONG_MAX);

        TEST_ASSERT_TRUE(p->home->is_billing);
        TEST_ASSERT_TRUE(text_equals(p->home->street1, "street 1"));
        TEST_ASSERT_TRUE(text_equals(p->home->street2, "street 2"));
        TEST_ASSERT_TRUE(text_equals(p->home->security->type, "silent"));
        TEST_ASSERT_TRUE(text_equals(p->home->security->name, "Vanguard"));

        TEST_ASSERT_PTR_NOT_NULL(p->addresses);
        TEST_ASSERT_EQUAL(list_count(p->addresses), 2);
#endif

        //    address* add1 = list_get_item(p->addresses, 0)->value;
#ifdef CHECK
        TEST_ASSERT_FALSE(add1->is_billing);
        TEST_ASSERT_TRUE(text_equals(add1->street1, "A list street 1"));
        TEST_ASSERT_TRUE(text_equals(add1->street2, "A list street 2"));
        TEST_ASSERT_TRUE(text_equals(add1->security->type, "A Alarm Type"));
        TEST_ASSERT_TRUE(text_equals(add1->security->name, "A Alarm Name"));
#endif

        //      address* add2 = list_get_item(p->addresses, 1)->value;
#ifdef CHECK
        TEST_ASSERT_FALSE(add1->is_billing);
        TEST_ASSERT_TRUE(text_equals(add2->street1, "B list street 1"));
        TEST_ASSERT_TRUE(text_equals(add2->street2, "B list street 2"));
        TEST_ASSERT_TRUE(text_equals(add2->security->type, "B Alarm Type"));
        TEST_ASSERT_TRUE(text_equals(add2->security->name, "B Alarm Name"));
#endif

        person_dispose(p);
    }

#ifdef CHECK
    xmemory_report_exit_on_leaks();
#endif

    return count;
}

benchmark json_benchmarks[] = {
    { "json_serialise_simple", json_serialise_simple },
    { "json_deserialise_simple", json_deserialise_simple },
    { "json_serialise_complex", json_serialise_complex },
    { "json_deserialise_complex", json_deserialise_complex },
    BENCHMARK_END,
};
