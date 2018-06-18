/*
* graph.h
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

#pragma once

#include "map.h"
#include "list.h"

typedef size_t graph_edge_id;
typedef size_t graph_vertex_id;

typedef struct {
    graph_vertex_id	id;
    char*			type;
    map*			attributes;
    void*			context;
} graph_vertex;

typedef struct {
    graph_edge_id	id;
    char*			type;
    graph_vertex*	a;
    graph_vertex*	b;
    map*			attributes;

    void*			context;
} graph_edge;

typedef struct {
    list*			verticies;
    list*			edges;
    map*			attributes;

    void*			context;

    graph_edge_id	edge_id_counter;
    graph_vertex_id	vertex_id_counter;
} graph;

graph*			graph_create();
void			graph_dispose(graph* g);

size_t			graph_count_vertices(graph* g);
size_t			graph_count_edges(graph* g);

graph_vertex*	graph_vertex_add(graph* g, char* type); // map* attributes);
bool			graph_vertex_remove(graph* g, graph_vertex* v);
void			graph_vertex_free(graph_vertex* v);
void			graph_vertex_attribute_set(graph_vertex* v, const char* name, const char* value);
char*			graph_vertex_attribute_get(graph_vertex* v, const char* name);
void			graph_vertex_attribute_remove(graph_vertex* v, const char* name);
graph_vertex*	graph_vertex_fetch(graph* g, graph_vertex_id vertex_id);
void			graph_vertex_attributes_set_append(graph_vertex* v, map* attributes);

graph_edge*		graph_edge_add(graph* g, graph_vertex* start, graph_vertex* end, const char* type);
bool			graph_edge_remove(graph* g, graph_edge* e);
void			graph_edge_free(graph_edge* e);
void			graph_edge_type_set(graph_edge* e, const char* type);
char*			graph_edge_type_get(graph_edge* e);
void			graph_edge_attribute_set(graph_edge* e, const char* name, const char* value);
char*			graph_edge_attribute_get(graph_edge* e, const char* name);
graph_edge*		graph_edge_fetch(graph* g, graph_vertex* v, graph_vertex* u, bool bidirectionally);
void			graph_edge_remove_link(graph* g, graph_vertex* v, graph_vertex* u);

list*			graph_vertex_fetch_edges_from(graph* g, graph_vertex_id vertex_id);
list*			graph_vertex_fetch_edges_to(graph* g, graph_vertex_id vertex_id);
list*			graph_vertex_fetch_edges(graph* g, graph_vertex_id vertex_id);

// char* graph_vertex_serialise(vertex* v);
// vertex* graph_vertex_deserialise(char* serialised);

