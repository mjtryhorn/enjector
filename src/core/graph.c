/*
* graph.c
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

// ref: http://en.wikipedia.org/wiki/Graph_theory

#include "enjector/core/xmemory.h"
#include "enjector/core/graph.h"

#include <assert.h>
#include "enjector/core/text.h"

graph* graph_create() {
    graph* g = xmemory_new(graph);
    g->verticies = list_create();
    g->edges = list_create();
    g->attributes = map_create();
    g->vertex_id_counter = 0;
    g->edge_id_counter = 0;

    return g;
}

void graph_dispose(graph* g) {
    assert(g);

    list_item** verticies = list_enumerable(g->verticies);
    int vertices_index = list_count(g->verticies);

    while (vertices_index-- > 0) {
        list_item* list_get_item = verticies[vertices_index];

        if (list_get_item->value) {
            graph_vertex* v = (graph_vertex*)list_get_item->value;
            graph_vertex_free(v);
        }
    }

    list_free(g->verticies);

    list_item** edges = list_enumerable(g->edges);
    int edges_index = list_count(g->edges);

    while (edges_index-- > 0) {
        list_item* list_get_item = edges[edges_index];

        if (list_get_item->value) {
            graph_edge* e = (graph_edge*)list_get_item->value;
            graph_edge_free(e);
        }
    }

    list_free(g->edges);
    map_dispose(g->attributes);

    xmemory_free(g);
}

size_t graph_count_vertices(graph* g) {
    assert(g);
    return list_count(g->verticies);
}

size_t graph_count_edges(graph* g) {
    assert(g);
    return list_count(g->edges);
}

graph_vertex* graph_vertex_add(graph* g, char* type) {
    assert(g);
    assert(type);

    graph_vertex* v = xmemory_new(graph_vertex);
    v->id = g->vertex_id_counter++;
    v->type = text_clone(type);
    v->attributes = map_create();
    list_add(g->verticies, v);

    return v;
}

bool graph_vertex_remove(graph* g, graph_vertex* v) {
    assert(g);
    assert(v);

    if (list_remove_item_by_value(g->verticies, v)) {

        // Remove edges
        list* edges_to = graph_vertex_fetch_edges_to(g, v->id);
        list_foreach_of_begin(edges_to, graph_edge*, e) {
            graph_edge_remove(g, e);
        }
        list_foreach_of_end;
        list_free(edges_to);

        list* edges_from = graph_vertex_fetch_edges_from(g, v->id);
        list_foreach_of_begin(edges_from, graph_edge*, e) {
            graph_edge_remove(g, e);
        }
        list_foreach_of_end;
        list_free(edges_from);

        graph_vertex_free(v);

        return true;
    }

    //  TODO: use list for each
    /*    list_item** verticies = list_enumerable(g->verticies);
    	int vertices_index = list_count(g->verticies);

    	while(vertices_index-- > 0) {
    		list_item* list_get_item = verticies[vertices_index];

    		if (list_get_item->value) {
    			graph_vertex* existing_verticle = (graph_vertex*) list_get_item->value;

    			if (existing_verticle == v) {
    				graph_vertex_free(existing_verticle);
    //                list_get_item->value = NULL;
    				list_remove_item_at(g->verticies, vertices_index);
    				break;
    			}
    		}
    	}*/
    return false;
}

void graph_vertex_free(graph_vertex* v) {
    assert(v);
    text_free(v->type);
    map_dispose(v->attributes);
    xmemory_free(v);
}

void graph_vertex_attribute_set(graph_vertex* v, const char* name, const char* value) {
    assert(v);
    assert(name);
    assert(value);

    map_set(v->attributes, name, text_clone(value));
}

char* graph_vertex_attribute_get(graph_vertex* v, const char* name) {
    assert(v);
    assert(name);

    return (char*)map_get_value(v->attributes, name);
}

graph_vertex* graph_vertex_fetch(graph* g, graph_vertex_id vertex_id) {
    assert(g);

    graph_vertex* found_vertex = NULL;

    list_item** verticies = list_enumerable(g->verticies);
    int vertices_index = list_count(g->verticies);

    while (vertices_index-- > 0) {
        list_item* list_get_item = verticies[vertices_index];

        if (list_get_item->value) {
            graph_vertex* v = (graph_vertex*)list_get_item->value;

            if (v->id == vertex_id) {
                found_vertex = v;
                break;
            }
        }
    }

    return found_vertex;
}

graph_edge* graph_edge_add(graph* g, graph_vertex* start, graph_vertex* end, const char* type) {
    assert(g);
    assert(start);
    assert(end);
    assert(type);

    if (graph_edge_fetch(g, start, end, false)) {
        return NULL;
    }

    graph_edge* e = xmemory_new(graph_edge);
    e->id = g->edge_id_counter++;
    e->type = text_clone(type);
    e->a = start;
    e->b = end;
    e->attributes = map_create();

    list_add(g->edges, e);

    return e;
}

bool graph_edge_remove(graph* g, graph_edge* e) {
    //list_item** edges = list_enumerable(g->edges);
//	int edges_index = list_count(g->edges);

    if (list_remove_item_by_value(g->edges, e)) {
        graph_edge_free(e);
        return true;
    }

    return false;

    //    while(edges_index-- > 0) {
    //        list_item* list_get_item = edges[edges_index];
    //
    //        if (list_get_item->value) {
    //            graph_edge* existing_edge = (graph_edge*) list_get_item->value;
    //
    //            if (existing_edge == e) {
    //                graph_edge_free(existing_edge);
    ////                list_get_item->value = NULL;
    //				list_remove_item_at(g->edges, edges_index);
    //                break;
    //            }
    //        }
    //    }
}

void graph_edge_free(graph_edge* e) {
    assert(e);
    assert(e->a);
    assert(e->b);

    text_free(e->type);
    map_dispose(e->attributes);

    xmemory_free(e);
}

void graph_edge_type_set(graph_edge* e, const char* type) {
    assert(e);
    assert(type);

    text_free(e->type);
    e->type = text_clone(type);
}

char* graph_edge_type_get(graph_edge* e) {
    assert(e);

    return e->type;
}

void graph_edge_attribute_set(graph_edge* e, const char* name, const char* value) {
    assert(e);
    assert(name);
    assert(value);

    map_set(e->attributes, name, text_clone(value));
}

char* graph_edge_attribute_get(graph_edge* e, const char* name) {
    assert(e);
    assert(name);

    return (char*)map_get_value(e->attributes, name);
}

graph_edge* graph_edge_fetch(graph* g, graph_vertex* v, graph_vertex* u, bool bidirectionally) {
    bool exists = false;
    list_foreach_of_begin(g->edges, graph_edge*, e) {
        if ((e->a == v && e->b == u) || (bidirectionally && e->a == u && e->b == v)) {
            return e;
            //			exists = true;
            //		break;
        }
    }
    list_foreach_of_end;

    return NULL;
}

void graph_edge_remove_link(graph* g, graph_vertex* v, graph_vertex* u) {
    list* edges = graph_vertex_fetch_edges_from(g, v->id);
    list_foreach_of_begin(edges, graph_edge*, e) {
        if (e->b->id == u->id) {
            graph_edge_remove(g, e);
        }
    }
    list_foreach_of_end;
    list_free(edges);
}

list* graph_vertex_fetch_edges_from(graph* g, graph_vertex_id vertex_id) {
    list* vertex_edges = list_create();

    list_foreach_of_begin(g->edges, graph_edge*, e) {
        if (e->a->id == vertex_id) {
            list_add(vertex_edges, e);
        }
    }
    list_foreach_of_end;

    //list_item** edges = list_enumerable(g->edges);
    //int edges_index = list_count(g->edges);

    //while (edges_index-- > 0) {
    //	list_item* list_get_item = edges[edges_index];

    //	if (list_get_item->value) {
    //		graph_edge* e = (graph_edge*)list_get_item->value;

    //		if (text_equals(e->a->id, graph_vertex_id)) {
    //			list_add(vertex_edges, e);
    //		}
    //	}
    //}

    return vertex_edges;
}

list* graph_vertex_fetch_edges_to(graph* g, graph_vertex_id vertex_id) {
    list* vertex_edges = list_create();

    list_foreach_of_begin(g->edges, graph_edge*, e) {
        if (e->b->id ==vertex_id) {
            list_add(vertex_edges, e);
        }
    }
    list_foreach_of_end;

    //list_item** edges = list_enumerable(g->edges);
    //int edges_index = list_count(g->edges);

    //while (edges_index-- > 0) {
    //	list_item* list_get_item = edges[edges_index];

    //	if (list_get_item->value) {
    //		graph_edge* e = (graph_edge*)list_get_item->value;

    //		if (text_equals(e->b->id, graph_vertex_id)) {
    //			list_add(vertex_edges, e);
    //		}
    //	}
    //}
    return vertex_edges;
}

list* graph_vertex_fetch_edges(graph* g, graph_vertex_id vertex_id) {
    list* vertex_edges = list_create();

    list_foreach_of_begin(g->edges, graph_edge*, e) {
        if (e->a->id == vertex_id || e->b->id == vertex_id) {
            list_add(vertex_edges, e);
        }
    }
    list_foreach_of_end;

    //list_item** edges = list_enumerable(g->edges);
    //int edges_index = list_count(g->edges);

    //while (edges_index-- > 0) {
    //	list_item* list_get_item = edges[edges_index];

    //	if (list_get_item->value) {
    //		graph_edge* e = (graph_edge*)list_get_item->value;

    //		if (text_equals(e->a->id, graph_vertex_id)) {
    //			list_add(vertex_edges, e);
    //		}
    //	}
    //}

    return vertex_edges;
}
