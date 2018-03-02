/*
* graph_benchmarks.c
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

#include "graph_benchmarks.h"

#include <enjector/core/graph.h>

#define MAX_NODES  100
#define MAX_LEVELS 100

int benchmark_create_graph(int argc, const char * argv[]) {

    graph* g = graph_create();
    graph_vertex* v = NULL;
    graph_vertex* v_prior = NULL;

    int count = 0;

    const int nodes = MAX_NODES;
    int levels = MAX_LEVELS;

    while(levels-->0) {
        int n = nodes;

        while(n-->0) {
            v = graph_vertex_add(g, "node");

            if (v_prior) {
                graph_edge_add(g, v_prior, v, "connection");
            }

            v_prior = v;
            count++;
        }
    }

    //graph_dispose(g);

    return count;
}

benchmark graph_benchmarks[] = {
    { "benchmark_create_graph", benchmark_create_graph },
    BENCHMARK_END,
};
