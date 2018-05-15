/*
* graph_tests.c
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

#include "graph_tests.h"

#include <enjector/core/graph.h>
#include "enjector/core/xmemory.h"
#include "enjector/core/text.h"

static void should_successfully_create_simple_graph() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v1);

    graph_vertex* v2 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v2);

    graph_edge* e1 = graph_edge_add(g, v1, v2, "connection");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->a);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->b);

    TEST_ASSERT_EQUAL_INT(1, graph_count_edges(g));
    TEST_ASSERT_EQUAL_INT(2, graph_count_vertices(g));

    TEST_ASSERT_EQUAL_PTR(v1, e1->a);
    TEST_ASSERT_EQUAL_PTR(v2, e1->b);

    graph_dispose(g);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_create_graph_with_attributes() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v1);

    graph_vertex* v2 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v2);

    graph_edge* e1 = graph_edge_add(g, v1, v2, "connection");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->a);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->b);

    graph_edge_attribute_set(e1, "is-a", "Dog");
    graph_edge_attribute_set(e1, "sound", "Bark");

    graph_vertex_attribute_set(v1, "name", "Fred");
    graph_vertex_attribute_set(v2, "name", "Paul");

    TEST_ASSERT_TRUE(text_equals("Dog", graph_edge_attribute_get(e1, "is-a")));
    TEST_ASSERT_TRUE(text_equals("Bark", graph_edge_attribute_get(e1, "sound")));
    TEST_ASSERT_TRUE(text_equals("Fred", graph_vertex_attribute_get(v1, "name")));
    TEST_ASSERT_TRUE(text_equals("Paul", graph_vertex_attribute_get(v2, "name")));

    graph_dispose(g);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_find_vertices() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v1);

    graph_vertex* v2 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v2);

    graph_edge* e1 = graph_edge_add(g, v1, v2, "connection");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->a);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->b);

    graph_vertex* fv1 = graph_vertex_fetch(g, v1->id);
    graph_vertex* fv2 = graph_vertex_fetch(g, v2->id);

    TEST_ASSERT_TRUE(v1 == fv1);
    TEST_ASSERT_TRUE(v2 == fv2);

    graph_dispose(g);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_find_child_edges() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    graph_vertex* v2 = graph_vertex_add(g, "node");
    graph_vertex* v3 = graph_vertex_add(g, "node");
    graph_edge* e1 = graph_edge_add(g, v1, v2, "connection");
    graph_edge* e2 = graph_edge_add(g, v1, v3, "connection");

    list* edges = graph_vertex_fetch_edges_from(g, v1->id);

    graph_edge* fe1 = ((graph_edge*) edges->data[0]->value);
    graph_edge* fe2 = ((graph_edge*) edges->data[1]->value);

    TEST_ASSERT_TRUE(e1 == fe1);
    TEST_ASSERT_TRUE(e2 == fe2);

    TEST_ASSERT_EQUAL_INT_FATAL(2, edges->length);
    TEST_ASSERT_EQUAL_INT(v2->id, fe1->b->id);
    TEST_ASSERT_EQUAL_INT(v3->id, fe2->b->id);

    list_free(edges);

    graph_dispose(g);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_find_parent_edges() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    graph_vertex* v2 = graph_vertex_add(g, "node");
    graph_vertex* v3 = graph_vertex_add(g, "node");
    graph_edge* e1 = graph_edge_add(g, v1, v3, "connection");
    graph_edge* e2 = graph_edge_add(g, v2, v3, "connection");

    list* edges = graph_vertex_fetch_edges_to(g, v3->id);

    graph_edge* fe1 = ((graph_edge*) edges->data[0]->value);
    graph_edge* fe2 = ((graph_edge*) edges->data[1]->value);

    TEST_ASSERT_TRUE(e1 == fe1);
    TEST_ASSERT_TRUE(e2 == fe2);

    TEST_ASSERT_EQUAL_INT_FATAL(2, edges->length);
    TEST_ASSERT_EQUAL_INT(v3->id, fe1->b->id);
    TEST_ASSERT_EQUAL_INT(v3->id, fe2->b->id);

    list_free(edges);

    graph_dispose(g);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_query_graph() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v1);

    graph_vertex* v2 = graph_vertex_add(g, "node");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(v2);

    graph_edge* e1 = graph_edge_add(g, v1, v2, "connection");
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->a);
    TEST_ASSERT_PTR_NOT_NULL_FATAL(e1->b);

    graph_edge_attribute_set(e1, "is-a", "Dog");
    graph_edge_attribute_set(e1, "sound", "Bark");

    graph_vertex_attribute_set(v1, "name", "Fred");
    graph_vertex_attribute_set(v2, "name", "Paul");

    TEST_ASSERT_TRUE(text_equals("Dog", graph_edge_attribute_get(e1, "is-a")));
    TEST_ASSERT_TRUE(text_equals("Bark", graph_edge_attribute_get(e1, "sound")));
    TEST_ASSERT_TRUE(text_equals("Fred", graph_vertex_attribute_get(v1, "name")));
    TEST_ASSERT_TRUE(text_equals("Paul", graph_vertex_attribute_get(v2, "name")));

    // TODO
    char* query = "/Animal/that[sound='bark']";

    graph_dispose(g);

    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_edge() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    graph_vertex* v2 = graph_vertex_add(g, "node");
    graph_vertex* v3 = graph_vertex_add(g, "node");
    graph_edge* e1 = graph_edge_add(g, v1, v3, "connection");
    graph_edge* e2 = graph_edge_add(g, v2, v3, "connection");

    graph_edge_remove_link(g, v2, v3);

    list* edges = graph_vertex_fetch_edges_to(g, v3->id);

    graph_edge* fe1 = ((graph_edge*)edges->data[0]->value);

    TEST_ASSERT_TRUE(e1 == fe1);

    TEST_ASSERT_EQUAL_INT(1, list_count(g->edges));
    TEST_ASSERT_EQUAL_INT(v3->id, fe1->b->id);

    list_free(edges);
    graph_dispose(g);
    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_single_part_bidirectional() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    graph_vertex* v2 = graph_vertex_add(g, "node");
    graph_vertex* v3 = graph_vertex_add(g, "node");
    graph_edge* e1 = graph_edge_add(g, v1, v3, "connection");
    graph_edge* e2 = graph_edge_add(g, v2, v3, "connection");
    graph_edge* e3 = graph_edge_add(g, v3, v2, "connection");

    graph_edge_remove_link(g, v2, v3);

    list* edges_from_v3 = graph_vertex_fetch_edges_from(g, v3->id);
    TEST_ASSERT_EQUAL_INT(1, list_count(edges_from_v3));

    graph_edge* fe3 = ((graph_edge*)edges_from_v3->data[0]->value);

    TEST_ASSERT_TRUE(e3 == fe3);

    TEST_ASSERT_EQUAL_INT(v3->id, fe3->a->id);
    TEST_ASSERT_EQUAL_INT(v2->id, fe3->b->id);

    TEST_ASSERT_EQUAL_INT(2, list_count(g->edges));

    list_free(edges_from_v3);
    graph_dispose(g);
    xmemory_report_exit_on_leaks();
}

static void should_successfully_remove_edges_after_vertex_removal() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    graph_vertex* v2 = graph_vertex_add(g, "node");
    graph_vertex* v3 = graph_vertex_add(g, "node");

    graph_vertex_remove(g, v3);

    list* edges = graph_vertex_fetch_edges_from(g, v1->id);
    TEST_ASSERT_EQUAL_INT(0, list_count(g->edges));
    list_free(edges);

    edges = graph_vertex_fetch_edges_from(g, v2->id);
    TEST_ASSERT_EQUAL_INT(0, list_count(g->edges));
    list_free(edges);

    graph_dispose(g);
    xmemory_report_exit_on_leaks();
}

static void should_fail_to_create_double_links() {
    graph* g = graph_create();
    TEST_ASSERT_PTR_NOT_NULL_FATAL(g);

    graph_vertex* v1 = graph_vertex_add(g, "node");
    graph_vertex* v2 = graph_vertex_add(g, "node");
    graph_vertex* v3 = graph_vertex_add(g, "node");
    graph_edge* e1 = graph_edge_add(g, v1, v3, "connection");
    graph_edge* e2 = graph_edge_add(g, v2, v3, "connection");
    graph_edge* e3 = graph_edge_add(g, v2, v3, "connection");

    TEST_ASSERT_PTR_NULL(e3);

    list* edges_from = graph_vertex_fetch_edges_from(g, v1->id);
    TEST_ASSERT_EQUAL_INT(1, list_count(edges_from));
    list_free(edges_from);

    edges_from = graph_vertex_fetch_edges_from(g, v2->id);
    TEST_ASSERT_EQUAL_INT(1, list_count(edges_from));
    list_free(edges_from);

    graph_dispose(g);
    xmemory_report_exit_on_leaks();
}

test graph_tests[] = {
    { "should_successfully_create_simple_graph", should_successfully_create_simple_graph },
    { "should_successfully_create_graph_with_attributes", should_successfully_create_graph_with_attributes },
    { "should_successfully_query_graph", should_successfully_query_graph },
    { "should_successfully_find_vertices", should_successfully_find_vertices },
    { "should_successfully_find_child_edges", should_successfully_find_child_edges },
    { "should_successfully_find_parent_edges", should_successfully_find_parent_edges },
    { "should_successfully_remove_edge", should_successfully_remove_edge },
    { "should_successfully_remove_single_part_bidirectional", should_successfully_remove_single_part_bidirectional },
    { "should_successfully_remove_edges_after_vertex_removal", should_successfully_remove_edges_after_vertex_removal },
    { "should_fail_to_create_double_links", should_fail_to_create_double_links },
    TEST_END
};
