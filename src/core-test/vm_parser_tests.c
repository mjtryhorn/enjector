/*
* vm_parser_tests.h
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

#include "vm_parser_tests.h"

#include <enjector/core/vm_parser.h>
#include <enjector/core/map.h>

#include <stdlib.h>

static void should_successfully_parse_single_function() {
    const char* text = "function test1() {}";

    script *script = vm_parser_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(script);

    bool result = vm_parser_load(script, text);
    TEST_ASSERT_TRUE_FATAL(result);
    TEST_ASSERT_EQUAL_INT(1, map_count(script->vm->functions));

    vm_parser_dispose(script);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_declare_variables() {
    const char* text = "function test1() { a = 1; }";

    script *script = vm_parser_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(script);

    bool result = vm_parser_load(script, text);
    TEST_ASSERT_TRUE_FATAL(result);
    //TEST_ASSERT_TRUE(list_count(script->bytecodes) > 0);

    vm_parser_dispose(script);

    xmemory_report_exit_on_leaks();
}


static void should_successfully_check_xpath_expression_parsing() {
    const char* xpath_statements[] = {
        "function test1() { a = /entity1; }",
        "function test1() { a = /entity1[amount == 1234.1]; }",
        "function test1() { a = /entity1[enabled == 1]/entity2[id == 1]; }",
        "function test1() { a = 'abc'; }",
        "function test1() { if (a == 'abc') { b = 1; } }",
        "function test1() { a = /entity1[firstname == \"abc\"]; }",
        NULL
    };


    int i = 0;
    const char* xpath_statement = NULL;

    while(xpath_statement = xpath_statements[i++], xpath_statement) {
        script *script = vm_parser_create();
        TEST_ASSERT_PTR_NOT_NULL_FATAL(script);

//        log("vm_parser_tests", "Parsing xpath statement: %s", xpath_statement);
        bool result = vm_parser_load(script, xpath_statement);
        TEST_ASSERT_TRUE_FATAL(result);
        //	TEST_ASSERT_TRUE(list_count(script->bytecodes) > 0);

        vm_parser_dispose(script);

        //	xmemory_report_exit_on_leaks();
    }

}

static void should_successfully_check_json_parsing() {
    const char* json_statements[] = {
//		"function test1() { a = {name:'fred'}; }",
//		"function test1() { a = {firstname:'fred', lastname:'smith'}; }",
//		"function test1() { a = {street:'123 The Street'}; b = {abc:a}; }",
//		"function test1() { a = {name:'fred',address:{street:123,town:456}}; }",
//		"function test1() { a = {name:'fred',address:{street:'123 The Street'}}; }",
        "function test1() { a = [1234, 456, 'fred', 3.14, {street:123,town:456}]; }",
        NULL
    };


    int i = 0;
    const char* json_statement = NULL;

    while(json_statement = json_statements[i++], json_statement) {
        script *script = vm_parser_create();
        TEST_ASSERT_PTR_NOT_NULL_FATAL(script);

        //log_debug("vm_parser_tests", "Parsing json statement: %s", json_statement);
        bool result = vm_parser_load(script, json_statement);
        TEST_ASSERT_TRUE_FATAL(result);
        //	TEST_ASSERT_TRUE(list_count(script->bytecodes) > 0);

        vm_parser_dispose(script);

        xmemory_report_exit_on_leaks();
    }

}


test vm_parser_tests[] = {
    { "should_successfully_parse_single_function", should_successfully_parse_single_function },
    { "should_successfully_declare_variables", should_successfully_declare_variables },
//    { "should_successfully_check_xpath_expression_parsing", should_successfully_check_xpath_expression_parsing },
//	{ "should_successfully_check_json_parsing", should_successfully_check_json_parsing },
    TEST_END
};


//
//static const char* schema = "
//	{
//	tables: [
//			 {
//			 name: 'person',
//			 columns: [
//					   {
//					   name: 'firstname',
//					   type: 'string',
//					   required: true
//					   },
//					   {
//					   name: 'lastname',
//					   type: 'string'
//					   },
//					   {
//					   name: 'age',
//					   type: 'number'
//					   },
//					   {
//					   name: 'enabled',
//					   type: 'bool'
//					   }
//					   ]
//			 },
//			 {
//			 name: 'activity',
//			 columns: [
//					   {
//					   name: 'message',
//					   type: 'string',
//					   required: true
//					   },
//					   {
//					   name: 'type',
//					   type: 'number'
//					   },
//					   {
//					   name: 'created_on',
//					   type: 'timestamp'
//					   },
//					   {
//					   name: 'created_by',
//					   type: 'ref',
//					   table: 'person'
//					   }
//					   ]
//			 },
//			 {
//			 name: 'event',
//			 columns: [
//					   {
//					   name: 'name',
//					   type: 'string',
//					   required: true
//					   },
//					   {
//					   name: 'type',
//					   type: 'number'
//					   },
//					   {
//					   name: 'created_on',
//					   type: 'timestamp'
//					   },
//					   {
//					   name: 'activity_id',
//					   type: 'ref',
//					   table: 'activity'
//					   }
//					   ]
//			 }
//			 ]
//	}";

//it("should compile error if xquery doesn't begin with //", async(function () {
//		var xquery = "/person";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, xquery);
//		}).toThrow('Expected "/" but "p" found.');
//	}));
//
//	it("should successfully compile if xquery begins with // and a single entity", async(function () {
//		var query = "//person";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person"}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with two entities specified", async(function () {
//		var query = "//person/activity";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person"},
//																			 {"name": "activity"}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				//expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with three entities specified", async(function () {
//		var query = "//person/activity/event";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person"},
//																			 {"name": "activity"},
//																			 {"name": "event"}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.id, t3.* FROM person t1, activity t2, event t3 WHERE t2.created_by = t1.id AND t3.activity_id = t2.id");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should fail to compile with an invalid entity relationship", async(function () {
//		var query = "//activity/person";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("no relationship between 'activity' and 'person'");
//	}));
//
//	it("should fail to compile with one invalid entity", async(function () {
//		var query = "//person/zippy";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("entity 'zippy' doesn't exist");
//	}));
//
//	it("should fail to compile with a single entity with a condition with a non existing attribute name", async(function () {
//		var query = "//person[doesntexist=true]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("attribute 'person.doesntexist' doesn't exist");
//	}));
//
//	it("should successfully compile with an empty condition", async(function () {
//		var query = "//person[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person"}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with an empty condition followed by a filtered entity", async(function () {
//		var query = "//person[]/activity[created_on > 1/1/2015]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person"},
//																			 {"name": "activity", "conditions": [
//																												 {"type": "attribute", "left": "created_on", "operator": "gt", "right": {"type": "timestamp", "value": "1/1/2015"}}
//																												 ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id AND t2.created_on > '1/1/2015'::timestamp without time zone");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with a like expression", async(function () {
//		var query = "//person[firstname like '%a']";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "firstname", "operator": "contains", "right": {"type": "string", "value": "%a"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.firstname like '%a'");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with a single entity with a condition with a single name", async(function () {
//		var query = "//person[enabled=true]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "enabled", "operator": "eq", "right": {"type": "bool", "value": "true"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.enabled = true");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with a single entity with a condition with a name and number", async(function () {
//		var query = "//person[age>30]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "number", "value": "30"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.age > 30");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with a two entities with a conditions each with a name and number", async(function () {
//		var query = "//person[age>30]/activity[type>1]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "number", "value": "30"}}
//																											   ]},
//																			 {"name": "activity", "conditions": [
//																												 {"type": "attribute", "left": "type", "operator": "gt", "right": {"type": "number", "value": "1"}}
//																												 ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id AND t1.age > 30 AND t2.type > 1");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with a with a logically combined conditions", async(function () {
//		var query = "//person[age>30 AND  age>10 OR age < 10]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "number", "value": "30"}},
//																											   {"type": "logic", "operator": "AND"},
//																											   {"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "number", "value": "10"}},
//																											   {"type": "logic", "operator": "OR"},
//																											   {"type": "attribute", "left": "age", "operator": "lt", "right": {"type": "number", "value": "10"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.age > 30 AND t1.age > 10 OR t1.age < 10");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile comparison with text value", async(function () {
//		var query = "//person[firstname = 'Fred Smith 1234 says, \"Hello\"']";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "firstname", "operator": "eq", "right": {"type": "string", "value": "Fred Smith 1234 says, \"Hello\""}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.firstname = 'Fred Smith 1234 says, \"Hello\"'");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile a single placeholder", async(function () {
//		var query = "//person[firstname = $1]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "firstname", "operator": "eq", "right": {"type": "placeholder", "value": "$1"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.firstname = $1");
//				await_promise(driver.query(sql, ['Fred']));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile a multiple placeholders", async(function () {
//		var query = "//person[firstname = $1 AND age > $2 AND enabled = $3]/activity[created_on > $4]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "firstname", "operator": "eq", "right": {"type": "placeholder", "value": "$1"}},
//																											   {"type": "logic", "operator": "AND"},
//																											   {"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "placeholder", "value": "$2"}},
//																											   {"type": "logic", "operator": "AND"},
//																											   {"type": "attribute", "left": "enabled", "operator": "eq", "right": {"type": "placeholder", "value": "$3"}}
//																											   ]},
//																			 {"name": "activity", "conditions": [
//																												 {"type": "attribute", "left": "created_on", "operator": "gt", "right": {"type": "placeholder", "value": "$4"}}
//																												 ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id AND t1.firstname = $1 AND t1.age > $2 AND t1.enabled = $3 AND t2.created_on > $4");
//				await_promise(driver.query(sql, ['Fred', 10, true, '1/1/2015']));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify bool true value", async(function () {
//		var query = "//person[enabled = true]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "enabled", "operator": "eq", "right": {"type": "bool", "value": "true"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.enabled = true");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify bool false value", async(function () {
//		var query = "//person[enabled = false]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "enabled", "operator": "eq", "right": {"type": "bool", "value": "false"}}
//																											   ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 WHERE t1.enabled = false");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify date value", async(function () {
//		var query = "//activity[created_on > 01/01/2001]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "conditions": [
//																												 {"type": "attribute", "left": "created_on", "operator": "gt", "right": {"type": "timestamp", "value": "01/01/2001"}}
//																												 ]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 WHERE t1.created_on > '01/01/2001'::timestamp without time zone");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should fail on compile due to invalue date value type", async(function () {
//		var query = "//activity[created_on > '01/01/2001']";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("invalid type comparison. Cannot compare 'activity.created_on' of type 'timestamp' doesn't match right side '01/01/2001' which has type 'string'");
//	}));
//
//	it("should successfully identify index position", async(function () {
//		var query = "//person[2]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "index": {"type": "position", "value": "2"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 LIMIT 1 OFFSET 2");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify first position", async(function () {
//		var query = "//person[first()]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "index": {"type": "first", "value": 1}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 LIMIT 1");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify range; skip start n items", async(function () {
//		var query = "//person[position()>=50]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "index": {"type": "skip", "value": "50"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 OFFSET 50");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify range; fetch range", async(function () {
//		var query = "//person[position() >= 10 AND  position()<=50]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "index": {"type": "range", "start": "10", "end": "50"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 LIMIT 40 OFFSET 10");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully identify range; fetch first n items", async(function () {
//		var query = "//person[position()<=50]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "index": {"type": "first", "value": "50"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM person t1 LIMIT 50");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully sort order desc; single attribute", async(function () {
//		var query = "//activity::sortDesc(created_on)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "sort": {"mode": "desc", "attributes": ["created_on"]}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 ORDER BY t1.created_on DESC");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should fail sort order desc on single invalid attribute", async(function () {
//		var query = "//activity::sortDesc(doesntexist)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("attribute 'activity.doesntexist' doesn't exist");
//	}));
//
//	it("should fail sort order desc on on invalid attribute after a valid one", async(function () {
//		var query = "//activity::sortDesc(type,doesntexist)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("attribute 'activity.doesntexist' doesn't exist");
//	}));
//
//	it("should successfully sort order desc; multiple attributes", async(function () {
//		var query = "//activity::sortDesc(created_on,type)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "sort": {"mode": "desc", "attributes": ["created_on", "type"]}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 ORDER BY t1.created_on DESC, t1.type DESC");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should fail compile due to range specification not on last entity", async(function () {
//		var query = "//person[position()<=10]/activity::sortDesc(created_on)[]";
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow('range specification can only be applied on the last entity');
//	}));
//
//	it("should fail compile due to range specification on more than one entity", async(function () {
//		var query = "//person[position()<=10]/activity[position() <= 10]";
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow('multiple ranges specified, only the last entity can have a range specification');
//	}));
//
//	it("should successfully sort order desc; fetch newest items sorted by single attribute", async(function () {
//		var query = "//activity::sortDesc(created_on)[position() <= 10]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "sort": {"mode": "desc", "attributes": ["created_on"]}, "index": {"type": "first", "value": "10"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 ORDER BY t1.created_on DESC LIMIT 10");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully sort order desc; fetch newest items sorted by multiple attributes", async(function () {
//		var query = "//person[firstname = 'Fred']/activity::sortDesc(created_on,type)[position() <= 10]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "firstname", "operator": "eq", "right": {"type": "string", "value": "Fred"}}
//																											   ]},
//																			 {"name": "activity", "sort": {"mode": "desc", "attributes": ["created_on", "type"]}, "index": {"type": "first", "value": "10"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id AND t1.firstname = 'Fred' ORDER BY t2.created_on DESC, t2.type DESC LIMIT 10");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully sort order asc; single attribute", async(function () {
//		var query = "//activity::sortAsc(created_on)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "sort": {"mode": "asc", "attributes": ["created_on"]}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 ORDER BY t1.created_on");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should fail sort order asc on single invalid attribute", async(function () {
//		var query = "//activity::sortAsc(doesntexist)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("attribute 'activity.doesntexist' doesn't exist");
//	}));
//
//	it("should fail sort order asc on on invalid attribute after a valid one", async(function () {
//		var query = "//activity::sortAsc(type,doesntexist)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//		}).toThrow("attribute 'activity.doesntexist' doesn't exist");
//	}));
//
//	it("should successfully sort order asc; multiple attributes", async(function () {
//		var query = "//activity::sortAsc(created_on,type)[]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "sort": {"mode": "asc", "attributes": ["created_on", "type"]}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 ORDER BY t1.created_on, t1.type");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully sort order asc; fetch newest items sorted by single attribute", async(function () {
//		var query = "//activity::sortAsc(created_on)[position() <= 10]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "activity", "sort": {"mode": "asc", "attributes": ["created_on"]}, "index": {"type": "first", "value": "10"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.* FROM activity t1 ORDER BY t1.created_on LIMIT 10");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully sort order asc; fetch newest items sorted by multiple attributes", async(function () {
//		var query = "//person[firstname = 'Fred']/activity::sortAsc(created_on,type)[position() <= 10]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "conditions": [
//																											   {"type": "attribute", "left": "firstname", "operator": "eq", "right": {"type": "string", "value": "Fred"}}
//																											   ]},
//																			 {"name": "activity", "sort": {"mode": "asc", "attributes": ["created_on", "type"]}, "index": {"type": "first", "value": "10"}}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id AND t1.firstname = 'Fred' ORDER BY t2.created_on, t2.type LIMIT 10");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully sort order asc on one entity and desc on another", async(function () {
//		var query = "//person::sortAsc(lastname)[firstname = 'Fred' AND enabled = true]/activity::sortAsc(created_on,type)[created_on > 01/01/2014]";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "list", "entities": [
//																			 {"name": "person", "sort": {"mode": "asc", "attributes": ["lastname"]}, "conditions": [
//																																									{"type": "attribute", "left": "firstname", "operator": "eq", "right": {"type": "string", "value": "Fred"}},
//																																									{"type": "logic", "operator": "AND"},
//																																									{"type": "attribute", "left": "enabled", "operator": "eq", "right": {"type": "bool", "value": "true"}}
//																																									]},
//																			 {"name": "activity", "sort": {"mode": "asc", "attributes": ["created_on", "type"]}, "conditions": [
//																																												{"type": "attribute", "left": "created_on", "operator": "gt", "right": {"type": "timestamp", "value": "01/01/2014"}}
//																																												]}
//																			 ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT t1.id, t2.* FROM person t1, activity t2 WHERE t2.created_by = t1.id AND t1.firstname = 'Fred' AND t1.enabled = true AND t2.created_on > '01/01/2014'::timestamp without time zone ORDER BY t1.lastname, t2.created_on, t2.type");
//				await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully compile with count expression with a logically combined conditions", async(function () {
//		var query = "count(//person::sortAsc(age)[age>30 AND  age>10 OR age < 10])";
//		var sql = null;
//
//		expect(function () {
//			var compiled = xpath.xpathCompile(schema, query);
//			expect(compiled.parseTree).toEqual({"type": "count", "entities": [
//																			  {"name": "person", "sort": {"mode": "asc", "attributes": ["age"]}, "conditions": [
//																																								{"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "number", "value": "30"}},
//																																								{"type": "logic", "operator": "AND"},
//																																								{"type": "attribute", "left": "age", "operator": "gt", "right": {"type": "number", "value": "10"}},
//																																								{"type": "logic", "operator": "OR"},
//																																								{"type": "attribute", "left": "age", "operator": "lt", "right": {"type": "number", "value": "10"}}
//																																								]}
//																			  ]});
//			sql = compiled.sql;
//		}).not.toThrow();
//
//		if (sql) {
//			expect(function () {
//				expect(sql).toEqual("SELECT COUNT(t1.id) as count FROM person t1 WHERE t1.age > 30 AND t1.age > 10 OR t1.age < 10");
//				var result = await_promise(driver.query(sql));
//			}).not.toThrow();
//		}
//	}));
//
//	it("should successfully run with count expression", async(function () {
//		var query = "count(//person::sortAsc(age)[age>30])";
//
//		expect(function () {
//			await (driver.query('INSERT INTO person(id, firstname, lastname, age) VALUES ($1, $2, $3, $4)', [uuid.v1(), 'Fred1', 'Smith1', 5]));
//			await (driver.query('INSERT INTO person(id, firstname, lastname, age) VALUES ($1, $2, $3, $4)', [uuid.v1(), 'Fred2', 'Smith2', 35]));
//
//			var result = await_promise(xpath.xquery(schema, query));
//			expect(result).not.toBeUndefined();
//			expect(result).not.toBeNull();
//			expect(result.rowCount).toBe(1);
//			expect(result.rows[0].count).toBe('1');
//		}).not.toThrow();
//	}));
//});
