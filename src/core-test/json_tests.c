/*
* json_tests.c
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

#include "json_tests.h"

#include <enjector/core/json.h>
#include <enjector/core/clock.h>

#include <limits.h>

#ifndef _MSC_VER
// Examples would be different on UNIX
#define LONG_MAX 2147483647
#define LONG_MIN -2147483648
#endif

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

    float score;
} person;

serialisable(person, {
    as_bool(enabled);
    as_string(firstname);
    as_string(lastname);
    as_integer(age);
    as_long(created_on);

    as_object(address, home);
    as_list(address, addresses);

    as_float(score);
});

static void should_successfully_json_serialise_object() {
    person* p = person_create();

    p->enabled = true;
    p->firstname = text_clone("Fred");
    p->lastname = text_clone("Smith");
    p->created_on = LONG_MAX;
    p->age = 42;

    p->home = address_create();
    p->home->is_billing = true;
    p->home->street1 = text_clone("street 1");
    p->home->street2 = text_clone("street 2");
    p->home->security = security_create();
    p->home->security->type = text_clone("silent");
    p->home->security->name = text_clone("Vanguard");

    p->addresses = list_create();
    p->score = 1.123456f;

    address* add1 = address_create();
    add1->is_billing = true;
    add1->street1 = text_clone("A list street 1");
    add1->street2 = text_clone("A list street 2");
    add1->security = security_create();
    add1->security->type = text_clone("A Alarm Type");
    add1->security->name = text_clone("A Alarm Name");
    list_add(p->addresses, add1);

    address *add2 = address_create();
    add2->is_billing = false;
    add2->street1 = text_clone("B list street 1");
    add2->street2 = text_clone("B list street 2");
    add2->security = security_create();
    add2->security->type = text_clone("B Alarm Type");
    add2->security->name = text_clone("B Alarm Name");
    list_add(p->addresses, add2);

    string_buffer* json = person_serialise(p);

    TEST_ASSERT_TRUE(text_equals(json->data, "{\"enabled\":true,\"firstname\":\"Fred\",\"lastname\":\"Smith\",\"age\":42,\"created_on\":2147483647,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":[{\"is_billing\":true,\"street1\":\"A list street 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":false,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}],\"score\":1.123456}"));

    string_buffer_free(json);

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object() {
    char* json = "{\"firstname\":\"Fred\",\"lastname\":\"Smith\",\"age\":42,\"created_on\":2147483647,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":[{\"street1\":\"A list street 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":true,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}],\"score\":1.123456}";

    person* p = person_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(p);

    TEST_ASSERT_TRUE(text_equals(p->firstname, "Fred"));
    TEST_ASSERT_TRUE(text_equals(p->lastname, "Smith"));
    TEST_ASSERT_EQUAL_SIZE(p->age, 42);
    TEST_ASSERT_EQUAL_LONG(p->created_on, LONG_MAX);

    TEST_ASSERT_TRUE(p->home->is_billing);
    TEST_ASSERT_TRUE(text_equals(p->home->street1, "street 1"));
    TEST_ASSERT_TRUE(text_equals(p->home->street2, "street 2"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->type, "silent"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->name, "Vanguard"));

    TEST_ASSERT_PTR_NOT_NULL(p->addresses);
    TEST_ASSERT_EQUAL_SIZE(list_count(p->addresses), 2);

    address* add1 = list_get_item(p->addresses, 0)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add1->street1, "A list street 1"));
    TEST_ASSERT_TRUE(text_equals(add1->street2, "A list street 2"));
    TEST_ASSERT_TRUE(text_equals(add1->security->type, "A Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add1->security->name, "A Alarm Name"));

    address* add2 = list_get_item(p->addresses, 1)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add2->street1, "B list street 1"));
    TEST_ASSERT_TRUE(text_equals(add2->street2, "B list street 2"));
    TEST_ASSERT_TRUE(text_equals(add2->security->type, "B Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add2->security->name, "B Alarm Name"));

    TEST_ASSERT_EQUAL_FLOAT(1.123456f, p->score);

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_serialise_object_with_foreign_charset() {
    person* p = person_create();

    p->enabled = true;
    p->firstname = text_clone("murciélago");
    p->lastname = text_clone("たとえば");
    p->age = 42;
    p->created_on = LONG_MIN;

    p->home = address_create();
    p->home->is_billing = true;
    p->home->street1 = text_clone("street 1");
    p->home->street2 = text_clone("street 2");
    p->home->security = security_create();
    p->home->security->type = text_clone("silent");
    p->home->security->name = text_clone("Vanguard");

    p->addresses = list_create();

    address* add1 = address_create();
    add1->is_billing = true;
    add1->street1 = text_clone("A list street 1");
    add1->street2 = text_clone("A list street 2");
    add1->security = security_create();
    add1->security->type = text_clone("A Alarm Type");
    add1->security->name = text_clone("A Alarm Name");
    list_add(p->addresses, add1);

    address *add2 = address_create();
    add2->is_billing = false;
    add2->street1 = text_clone("B list street 1");
    add2->street2 = text_clone("B list street 2");
    add2->security = security_create();
    add2->security->type = text_clone("B Alarm Type");
    add2->security->name = text_clone("B Alarm Name");
    list_add(p->addresses, add2);

    string_buffer* json = person_serialise(p);

    TEST_ASSERT_TRUE(text_equals(json->data, "{\"enabled\":true,\"firstname\":\"murciélago\",\"lastname\":\"たとえば\",\"age\":42,\"created_on\":-2147483648,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":[{\"is_billing\":true,\"street1\":\"A list street 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":false,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}],\"score\":0.000000}"));

    string_buffer_free(json);

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object_with_foreign_charset() {
    char* json = "{\"firstname\":\"murciélago\",\"lastname\":\"たとえば\",\"age\":42,\"created_on\":-2147483648,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":[{\"street1\":\"A list street 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":true,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}]}";

    person* p = person_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(p);

    TEST_ASSERT_TRUE(text_equals(p->firstname, "murciélago"));
    TEST_ASSERT_TRUE(text_equals(p->lastname, "たとえば"));
    TEST_ASSERT_EQUAL_SIZE(p->age, 42);
    TEST_ASSERT_EQUAL_LONG(p->created_on, LONG_MIN);
    TEST_ASSERT_EQUAL_FLOAT(p->score, 0);

    TEST_ASSERT_TRUE(p->home->is_billing);
    TEST_ASSERT_TRUE(text_equals(p->home->street1, "street 1"));
    TEST_ASSERT_TRUE(text_equals(p->home->street2, "street 2"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->type, "silent"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->name, "Vanguard"));

    TEST_ASSERT_PTR_NOT_NULL(p->addresses);
    TEST_ASSERT_EQUAL_SIZE(list_count(p->addresses), 2);

    address* add1 = list_get_item(p->addresses, 0)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add1->street1, "A list street 1"));
    TEST_ASSERT_TRUE(text_equals(add1->street2, "A list street 2"));
    TEST_ASSERT_TRUE(text_equals(add1->security->type, "A Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add1->security->name, "A Alarm Name"));

    address* add2 = list_get_item(p->addresses, 1)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add2->street1, "B list street 1"));
    TEST_ASSERT_TRUE(text_equals(add2->street2, "B list street 2"));
    TEST_ASSERT_TRUE(text_equals(add2->security->type, "B Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add2->security->name, "B Alarm Name"));

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}


static void should_successfully_json_serialise_object_with_json_chars() {
    person* p = person_create();

    p->enabled = true;
    p->firstname = text_clone("Fred'so");
    p->lastname = text_clone("Smith\"so");
    p->age = 42;
    p->created_on = LONG_MAX;

    p->home = address_create();
    p->home->is_billing = true;
    p->home->street1 = text_clone("{street 1}");
    p->home->street2 = text_clone("street 2},");
    p->home->security = security_create();
    p->home->security->type = text_clone("{silent");
    p->home->security->name = text_clone("Va\"ngu\"ard");

    p->addresses = list_create();

    address* add1 = address_create();
    add1->is_billing = true;
    add1->street1 = text_clone("'A [l]i:s{t str}ee,t 1");
    add1->street2 = text_clone("A list street 2");
    add1->security = security_create();
    add1->security->type = text_clone("A Alarm Type");
    add1->security->name = text_clone("A Alarm Name");
    list_add(p->addresses, add1);

    address *add2 = address_create();
    add2->is_billing = false;
    add2->street1 = text_clone("B list street 1");
    add2->street2 = text_clone("B list street 2");
    add2->security = security_create();
    add2->security->type = text_clone("B Alarm Type");
    add2->security->name = text_clone("B Alarm Name");
    list_add(p->addresses, add2);

    string_buffer* json = person_serialise(p);

    TEST_ASSERT_TRUE(text_equals(json->data, "{\"enabled\":true,\"firstname\":\"Fred'so\",\"lastname\":\"Smith\\\"so\",\"age\":42,\"created_on\":2147483647,\"home\":{\"is_billing\":true,\"street1\":\"{street 1}\",\"street2\":\"street 2},\",\"security\":{\"type\":\"{silent\",\"name\":\"Va\\\"ngu\\\"ard\"}},\"addresses\":[{\"is_billing\":true,\"street1\":\"'A [l]i:s{t str}ee,t 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":false,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}],\"score\":0.000000}"));

    string_buffer_free(json);

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object_with_json_chars() {
    char* json = "{\"enabled\":true,\"firstname\":\"Fred'so\",\"lastname\":\"Smith\\\"so\",\"age\":42,\"created_on\":-2147483648,\"home\":{\"is_billing\":true,\"street1\":\"{street 1}\",\"street2\":\"street 2},\",\"security\":{\"type\":\"{silent\",\"name\":\"Va\\\"ngu\\\"ard\"}},\"addresses\":[{\"is_billing\":false,\"street1\":\"'A [l]i:s{t str}ee,t 1\",\"street2\":\"A list street 2\",\"security\":{\"type\":\"A Alarm Type\",\"name\":\"A Alarm Name\"}},{\"is_billing\":false,\"street1\":\"B list street 1\",\"street2\":\"B list street 2\",\"security\":{\"type\":\"B Alarm Type\",\"name\":\"B Alarm Name\"}}]}";

    person* p = person_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(p);

    TEST_ASSERT_TRUE(text_equals(p->firstname, "Fred'so"));
    TEST_ASSERT_TRUE(text_equals(p->lastname, "Smith\"so"));
    TEST_ASSERT_EQUAL_SIZE(p->age, 42);
    TEST_ASSERT_EQUAL_LONG(p->created_on, LONG_MIN);

    TEST_ASSERT_TRUE(p->home->is_billing);
    TEST_ASSERT_TRUE(text_equals(p->home->street1, "{street 1}"));
    TEST_ASSERT_TRUE(text_equals(p->home->street2, "street 2},"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->type, "{silent"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->name, "Va\"ngu\"ard"));

    TEST_ASSERT_PTR_NOT_NULL(p->addresses);
    TEST_ASSERT_EQUAL_SIZE(list_count(p->addresses), 2);

    address* add1 = list_get_item(p->addresses, 0)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add1->street1, "'A [l]i:s{t str}ee,t 1"));
    TEST_ASSERT_TRUE(text_equals(add1->street2, "A list street 2"));
    TEST_ASSERT_TRUE(text_equals(add1->security->type, "A Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add1->security->name, "A Alarm Name"));

    address* add2 = list_get_item(p->addresses, 1)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add2->street1, "B list street 1"));
    TEST_ASSERT_TRUE(text_equals(add2->street2, "B list street 2"));
    TEST_ASSERT_TRUE(text_equals(add2->security->type, "B Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add2->security->name, "B Alarm Name"));

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object_with_foreign_chars_and_json_chars_in_single_quotes() {
    char* json = "{'enabled':true,'firstname':'Fred\\'so','lastname':'Smith\"so','age':42,'created_on':0,'home':{'is_billing':true,'street1':'{street 1}','street2':'street 2},','security':{'type':'{silent','name':'Va\\'ngu\\'ard'}},'addresses':[{'is_billing':false,'street1':'\\'A [l]i:s{t str}ee,t 1','street2':'A list street 2','security':{'type':'A Alarm Type','name':'A Alarm Name'}},{'is_billing':false,'street1':'murciélago','street2':'たとえば','security':{'type':'B Alarm Type','name':'B Alarm Name'}}]}";

    person* p = person_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(p);

    TEST_ASSERT_TRUE(text_equals(p->firstname, "Fred'so"));
    TEST_ASSERT_TRUE(text_equals(p->lastname, "Smith\"so"));
    TEST_ASSERT_EQUAL_SIZE(p->age, 42);
    TEST_ASSERT_EQUAL_SIZE(p->created_on, 0);

    TEST_ASSERT_TRUE(p->home->is_billing);
    TEST_ASSERT_TRUE(text_equals(p->home->street1, "{street 1}"));
    TEST_ASSERT_TRUE(text_equals(p->home->street2, "street 2},"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->type, "{silent"));
    TEST_ASSERT_TRUE(text_equals(p->home->security->name, "Va'ngu'ard"));

    TEST_ASSERT_PTR_NOT_NULL_FATAL(p->addresses);
    TEST_ASSERT_EQUAL_SIZE(list_count(p->addresses), 2);

    address* add1 = list_get_item(p->addresses, 0)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add1->street1, "'A [l]i:s{t str}ee,t 1"));
    TEST_ASSERT_TRUE(text_equals(add1->street2, "A list street 2"));
    TEST_ASSERT_TRUE(text_equals(add1->security->type, "A Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add1->security->name, "A Alarm Name"));

    address* add2 = list_get_item(p->addresses, 1)->value;
    TEST_ASSERT_FALSE(add1->is_billing);
    TEST_ASSERT_TRUE(text_equals(add2->street1, "murciélago"));
    TEST_ASSERT_TRUE(text_equals(add2->street2, "たとえば"));
    TEST_ASSERT_TRUE(text_equals(add2->security->type, "B Alarm Type"));
    TEST_ASSERT_TRUE(text_equals(add2->security->name, "B Alarm Name"));

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}
typedef struct myaction_t {
    char* app;
    char* type;
    char* name;
    char* message_id;
    char* created_on;
    char* source;
    char* payload;
} myaction;

serialisable(myaction, {
    as_string(app);
    as_string(type);
    as_string(name);
    as_string(message_id);
    as_string(created_on);
    as_string(source);
    as_string(payload);
});

static void should_successfully_json_deserialise_object_extract_payload_without_deserialising() {
    char* json = "{\"app\":\"enjector\", \"type\" : \"session\", \"name\" : \"session authenticate\", \"message_id\" : \"b726191b - 5dfd - 979e-73bf - 1fc877c830ae\", \"created_on\" : \"Fri, 21 Aug 2015[ ] { ' } null true false : 10:17:36 GMT\", \"source\" : \"client\", \"payload\" : {\"username\":\"a\", \"password\" : \"a\"}}";

    myaction* a = myaction_deserialise(json);

    TEST_ASSERT_TRUE(text_equals(a->app, "enjector"));
    TEST_ASSERT_TRUE(text_equals(a->type, "session"));
    TEST_ASSERT_TRUE(text_equals(a->name, "session authenticate"));
    TEST_ASSERT_TRUE(text_equals(a->message_id, "b726191b - 5dfd - 979e-73bf - 1fc877c830ae"));
    TEST_ASSERT_TRUE(text_equals(a->created_on, "Fri, 21 Aug 2015[ ] { ' } null true false : 10:17:36 GMT"));
    TEST_ASSERT_TRUE(text_equals(a->source, "client"));
    TEST_ASSERT_TRUE(text_equals(a->payload, "{\"username\":\"a\", \"password\" : \"a\"}"));

    myaction_dispose(a);

    xmemory_report_exit_on_leaks();
}

typedef struct uidpwd_t {
    char* username;
    char* password;
    list* group_names;
    list* group_ids;
} uidpwd;

serialisable(uidpwd, {
    as_string(username);
    as_string(password);
});

static void should_successfully_json_deserialise_object_with_just_nulls() {
    char* json = "{\"username\":null, \"password\" : null}";

    uidpwd* a = uidpwd_deserialise(json);

    TEST_ASSERT_PTR_NULL(a->username);
    TEST_ASSERT_PTR_NULL(a->password);

    uidpwd_dispose(a);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_serialise_object_with_nulls() {
    const char* expected_json = "{\"username\":null,\"password\":null}";

    uidpwd* a = uidpwd_create();
//    TEST_ASSERT_PTR_NULL(a->username);
    //  TEST_ASSERT_PTR_NULL(a->password);

    string_buffer* json = uidpwd_serialise(a);

    TEST_ASSERT_TRUE(text_equals(expected_json, json->data));

    string_buffer_free(json);
    uidpwd_dispose(a);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object_with_no_items() {
    person* p = person_create();

    p->enabled = true;
    p->firstname = text_clone("Fred");
    p->lastname = text_clone("Smith");
    p->age = 42;
    p->created_on = LONG_MAX;

    p->home = address_create();
    p->home->is_billing = true;
    p->home->street1 = text_clone("street 1");
    p->home->street2 = text_clone("street 2");
    p->home->security = security_create();
    p->home->security->type = text_clone("silent");
    p->home->security->name = text_clone("Vanguard");

    string_buffer* json = person_serialise(p);

    TEST_ASSERT_TRUE(text_equals(json->data, "{\"enabled\":true,\"firstname\":\"Fred\",\"lastname\":\"Smith\",\"age\":42,\"created_on\":2147483647,\"home\":{\"is_billing\":true,\"street1\":\"street 1\",\"street2\":\"street 2\",\"security\":{\"type\":\"silent\",\"name\":\"Vanguard\"}},\"addresses\":null,\"score\":0.000000}"));

    string_buffer_free(json);

    person_dispose(p);

    xmemory_report_exit_on_leaks();
}

static void should_fail_json_deserialise_object_with_no_quoted_names() {
    char* json = "{username:\"u1\", password: \"p1\"}";

    uidpwd* a = uidpwd_deserialise(json);
    TEST_ASSERT_PTR_NULL_FATAL(a);

    xmemory_report_exit_on_leaks();
}

typedef struct strings_holder_t {
    list* strings;
} strings_holder;

serialisable(strings_holder, {
    as_strings(strings);
});

static void should_successfully_serialise_a_list_of_strings() {
    strings_holder* holder = strings_holder_create();
    holder->strings = list_create();

    list_add(holder->strings, text_clone("string1"));
    list_add(holder->strings, text_clone("string2"));
    list_add(holder->strings, text_clone("string3"));

    string_buffer* json = strings_holder_serialise(holder);

    TEST_ASSERT_TRUE(text_equals(json->data, "{\"strings\":[\"string1\",\"string2\",\"string3\"]}"));

    string_buffer_free(json);

    strings_holder_dispose(holder);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_deserialise_a_list_of_strings() {
    char* json = "{\"strings\":[\"string1\",\"string2\",\"string3\"]}";

    strings_holder* holder = strings_holder_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(holder);

    TEST_ASSERT_EQUAL_SIZE(3, list_count(holder->strings));
    TEST_ASSERT_TRUE(text_equals("string1", (char*) list_get_item(holder->strings, 0)->value));
    TEST_ASSERT_TRUE(text_equals("string2", (char*) list_get_item(holder->strings, 1)->value));
    TEST_ASSERT_TRUE(text_equals("string3", (char*) list_get_item(holder->strings, 2)->value));

    strings_holder_dispose(holder);

    xmemory_report_exit_on_leaks();
}

typedef struct mixed_lists_holder_t {
    list* group_names;
    list* group_ids;
} mixed_lists_holder;

serialisable(mixed_lists_holder, {
    as_strings(group_names);
    as_integers(group_ids);
});

static void should_successfully_deserialise_a_mixed_list_of_strings_and_integers() {
    char* json = "{\"group_names\":[\"abc\",\"def\",\"ghijk\"], \"group_ids\":[1, 2112,33331, 4]}";

    mixed_lists_holder* holder = mixed_lists_holder_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(holder);

    TEST_ASSERT_EQUAL_SIZE(3, list_count(holder->group_names));
    TEST_ASSERT_TRUE(text_equals("abc", (char*)list_get_item(holder->group_names, 0)->value));
    TEST_ASSERT_TRUE(text_equals("def", (char*)list_get_item(holder->group_names, 1)->value));
    TEST_ASSERT_TRUE(text_equals("ghijk", (char*)list_get_item(holder->group_names, 2)->value));

    TEST_ASSERT_EQUAL_SIZE(4, list_count(holder->group_ids));
    TEST_ASSERT_EQUAL_SIZE(1, *((int*)list_get_item(holder->group_ids, 0)->value));
    TEST_ASSERT_EQUAL_SIZE(2112, *((int*)list_get_item(holder->group_ids, 1)->value));
    TEST_ASSERT_EQUAL_SIZE(33331, *((int*)list_get_item(holder->group_ids, 2)->value));
    TEST_ASSERT_EQUAL_SIZE(4, *((int*)list_get_item(holder->group_ids, 3)->value));

    mixed_lists_holder_dispose(holder);

    xmemory_report_exit_on_leaks();
}

static void should_fail_json_deserialise_due_to_parse_error() {
    char* json = "{\"app\":\"enjector\", \"type\" : \"session\", \"name\" : \"session authenticate\", \"message_id\" : \"b726191b - 5dfd - 979e-73bf - 1fc877c830ae\", \"created_on\" : \"Fri, 21 Aug 2015[ ] { ' } null true false : 10:17:36 GMT\", \"source\" : \"client\", \"payload\" : {\"username\":\"a\", \"password\" : \"a\"}}{\"app\":\"enjector\", \"type\" : \"session\", \"name\" : \"session authenticate\", \"message_id\" : \"b726191b - 5dfd - 979e-73bf - 1fc877c830ae\", \"created_on\" : \"Fri, 21 Aug 2015[ ] { ' } null true false : 10:17:36 GMT\", \"source\" : \"client\", \"payload\" : {\"username\":\"a\", \"password\" : \"a\"}}";

    myaction* a = myaction_deserialise(json);

    TEST_ASSERT_PTR_NULL(a);

    xmemory_report_exit_on_leaks();
}

typedef struct uidpwd2_t {
    char* username;
    char* password;
} uidpwd2;

serialisable(uidpwd2, {
    as_string(username);
    as_string(password);
    as_string(username);
});

static void should_successfully_json_serialisation_and_deserialise_object_with_duplicate_attributes() {
    uidpwd2* u = uidpwd2_create();

    u->username = text_clone("user1");
    u->password = text_clone("pass1");

    string_buffer* json = uidpwd2_serialise(u);
    TEST_ASSERT_TRUE(text_equals(json->data, "{\"username\":\"user1\",\"password\":\"pass1\",\"username\":\"user1\"}"));

    uidpwd2_dispose(u);
    string_buffer_free(json);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object_with_duplicate_attributes() {
    char* json = "{\"username\":\"user1\",\"password\":\"pass1\",\"username\":\"user2\"}";

    uidpwd2* q = uidpwd2_deserialise(json);
    TEST_ASSERT_PTR_NULL_FATAL(q);

    xmemory_report_exit_on_leaks();
}


typedef struct testinfo_t {
    int		from;
    int		to;
    int		term;
    bool success;
    int		match_index;
} testinfo;

serialisable(testinfo, {
    as_integer(from);
    as_integer(to);
    as_integer(term);
    as_bool(success);
    as_integer(match_index);
});

static void should_successfully_json_deserialise_object_with_bool1() {
    char* json = "{\"from\":26159,\"to\":26724,\"term\":3,\"success\":false,\"match_index\":-858993460}";

    testinfo* q = testinfo_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(q);

    TEST_ASSERT_EQUAL_SIZE(26159, q->from);
    TEST_ASSERT_EQUAL_SIZE(26724, q->to);
    TEST_ASSERT_EQUAL_SIZE(3, q->term);
    TEST_ASSERT_EQUAL_BOOL(false, q->success);
    TEST_ASSERT_FALSE(q->success);
    TEST_ASSERT_EQUAL_SIZE(-858993460, q->match_index);

    testinfo_dispose(q);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_object_with_bool2() {
    char* json = "{\"from\":26159,\"to\":26724,\"term\":3,\"success\":true,\"match_index\":-858993460}";

    testinfo* q = testinfo_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(q);

    TEST_ASSERT_EQUAL_SIZE(26159, q->from);
    TEST_ASSERT_EQUAL_SIZE(26724, q->to);
    TEST_ASSERT_EQUAL_SIZE(3, q->term);
    TEST_ASSERT_EQUAL_BOOL(true, q->success);
    TEST_ASSERT_TRUE(q->success);
    TEST_ASSERT_EQUAL_SIZE(-858993460, q->match_index);

    testinfo_dispose(q);

    xmemory_report_exit_on_leaks();
}

typedef struct test_cluster_discovery_info_t {
    int		id;
    char	group_name[100];
    char	group_name_shortcode[5];
    list*	hosts;
    int		port;
    char*	timestamp;
    char*	msg_id;
} test_cluster_discovery_info;

serialisable(test_cluster_discovery_info, {
    as_integer(id);
    as_char_array(group_name);
    as_char_array(group_name_shortcode);
    as_strings(hosts);
    as_integer(port);
    as_string(timestamp);
    as_string(msg_id);
});

static void should_successfully_json_deserialise_complex_object1() {
    char* json = "{\"id\":509052683,\"group_name\":\"test-cluster\",\"group_name_shortcode\":\"test-cluster\",\"hosts\":[\"127.0.0.1\",\"172.20.10.4\",\"25.31.117.166\",\"192.168.57.1\",\"10.211.55.2\",\"10.37.129.2\",\"172.16.65.1\",\"172.16.26.1\"],\"port\":4801,\"timestamp\":\"2016-02-02 17:08:50.781\",\"msg_id\":\"3b5e5289-f328-a213-c05e-2aabc9d813d1\"}";

    test_cluster_discovery_info* q = test_cluster_discovery_info_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(q);

    TEST_ASSERT_EQUAL_SIZE(509052683, q->id);
    TEST_ASSERT_TRUE(text_equals("test-cluster", q->group_name));
    TEST_ASSERT_TRUE(text_equals("test", q->group_name_shortcode));
    TEST_ASSERT_EQUAL_SIZE(4801, q->port);
    TEST_ASSERT_TRUE(text_equals("3b5e5289-f328-a213-c05e-2aabc9d813d1", q->msg_id));

    list* hosts = q->hosts;
    TEST_ASSERT_EQUAL_SIZE(8, list_count(hosts));
    TEST_ASSERT_TRUE(text_equals("127.0.0.1", (char*) list_get_item(hosts, 0)->value));

    test_cluster_discovery_info_dispose(q);

    xmemory_report_exit_on_leaks();
}

typedef struct test_theme_t {
    char*	name;
    int		votes;
} test_theme;

serialisable(test_theme, {
    as_string(name);
    as_integer(votes);
});

typedef struct test_theme_collection_t {
    char*	name;
    map*	themes;
} test_theme_collection;

serialisable(test_theme_collection, {
    as_string(name);
    as_map(test_theme, themes);
});

static void should_successfully_json_serialise_map() {
    char* expected_json = "{\"name\":\"Collection of themes\",\"themes\":{\"theme1\":{\"name\":\"theme1\",\"votes\":1},\"theme2\":{\"name\":\"theme2\",\"votes\":2}}}";

    test_theme_collection* collection = test_theme_collection_create();
    collection->name = text_clone("Collection of themes");
    collection->themes = map_create();

    test_theme* theme1 = test_theme_create();
    theme1->name = text_clone("theme1");
    theme1->votes = 1;
    map_set(collection->themes, theme1->name, theme1);

    test_theme* theme2 = test_theme_create();
    theme2->name = text_clone("theme2");
    theme2->votes = 2;
    map_set(collection->themes, theme2->name, theme2);

    string_buffer* actual_json = test_theme_collection_serialise(collection);

    TEST_ASSERT_TRUE(text_equals(expected_json, actual_json->data));

    string_buffer_free(actual_json);
    test_theme_collection_dispose(collection);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_json_deserialise_map() {
    char* json = "{\"name\":\"Collection of themes\",\"themes\":{\"theme1\":{\"name\":\"theme1\",\"votes\":1},\"theme2\":{\"name\":\"theme2\",\"votes\":2},\"theme3\":{\"name\":\"theme3\",\"votes\":3}}}";

    test_theme_collection* collection = test_theme_collection_deserialise(json);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(collection);

    TEST_ASSERT_TRUE(text_equals("Collection of themes", collection->name));
    TEST_ASSERT_EQUAL_SIZE_FATAL(3, map_count(collection->themes));

    map_item* item_theme1 = map_get_item_at(collection->themes, 0);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(item_theme1);
    TEST_ASSERT_TRUE(text_equals("theme1", item_theme1->name));
    test_theme* theme1 = (test_theme*) item_theme1->value;
    TEST_ASSERT_TRUE(text_equals("theme1", theme1->name));
    TEST_ASSERT_EQUAL_SIZE(1, theme1->votes);

    map_item* item_theme2 = map_get_item_at(collection->themes, 1);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(item_theme2);
    TEST_ASSERT_TRUE(text_equals("theme2", item_theme2->name));
    test_theme* theme2 = (test_theme*)item_theme2->value;
    TEST_ASSERT_TRUE(text_equals("theme2", theme2->name));
    TEST_ASSERT_EQUAL_SIZE(2, theme2->votes);

    map_item* item_theme3 = map_get_item_at(collection->themes, 2);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(item_theme3);
    TEST_ASSERT_TRUE(text_equals("theme3", item_theme3->name));
    test_theme* theme3 = (test_theme*)item_theme3->value;
    TEST_ASSERT_TRUE(text_equals("theme3", theme3->name));
    TEST_ASSERT_EQUAL_SIZE(3, theme3->votes);

    test_theme_collection_dispose(collection);

    xmemory_report_exit_on_leaks();
}

test json_tests[] = {
    {"should_successfully_json_serialise_object", should_successfully_json_serialise_object},
    {"should_successfully_json_deserialise_object", should_successfully_json_deserialise_object},
    { "should_successfully_json_serialise_object_with_foreign_charset", should_successfully_json_serialise_object_with_foreign_charset },
    { "should_successfully_json_deserialise_object_with_foreign_charset", should_successfully_json_deserialise_object_with_foreign_charset },
    { "should_successfully_json_serialise_object_with_json_chars", should_successfully_json_serialise_object_with_json_chars },
    { "should_successfully_json_deserialise_object_with_json_chars", should_successfully_json_deserialise_object_with_json_chars },
    { "should_successfully_json_deserialise_object_with_foreign_chars_and_json_chars_in_single_quotes", should_successfully_json_deserialise_object_with_foreign_chars_and_json_chars_in_single_quotes },
    { "should_successfully_json_deserialise_object_extract_payload_without_deserialising", should_successfully_json_deserialise_object_extract_payload_without_deserialising },
    { "should_successfully_json_deserialise_object_with_just_nulls", should_successfully_json_deserialise_object_with_just_nulls },
    { "should_successfully_json_serialise_object_with_nulls", should_successfully_json_serialise_object_with_nulls },
    { "should_successfully_json_deserialise_object_with_no_items", should_successfully_json_deserialise_object_with_no_items },
    { "should_fail_json_deserialise_object_with_no_quoted_names", should_fail_json_deserialise_object_with_no_quoted_names },
    { "should_successfully_serialise_a_list_of_strings", should_successfully_serialise_a_list_of_strings },
    { "should_successfully_deserialise_a_list_of_strings", should_successfully_deserialise_a_list_of_strings },
    { "should_successfully_deserialise_a_mixed_list_of_strings_and_integers", should_successfully_deserialise_a_mixed_list_of_strings_and_integers },
    { "should_fail_json_deserialise_due_to_parse_error", should_fail_json_deserialise_due_to_parse_error },
    { "should_successfully_json_serialisation_and_deserialise_object_with_duplicate_attributes", should_successfully_json_serialisation_and_deserialise_object_with_duplicate_attributes },
    { "should_successfully_json_deserialise_object_with_duplicate_attributes", should_successfully_json_deserialise_object_with_duplicate_attributes },
    { "should_successfully_json_deserialise_object_with_bool1", should_successfully_json_deserialise_object_with_bool1 },
    { "should_successfully_json_deserialise_object_with_bool2", should_successfully_json_deserialise_object_with_bool2 },
    { "should_successfully_json_deserialise_complex_object1", should_successfully_json_deserialise_complex_object1 },
    { "should_successfully_json_serialise_map", should_successfully_json_serialise_map },
    { "should_successfully_json_deserialise_map", should_successfully_json_deserialise_map },
    TEST_END
};
