#include "test.h"
#include "../include/adjacencyList.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

void test_al_bfs() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);
    al_add_vertex(&alist, 3);
    al_add_vertex(&alist, 4);
    al_add_vertex(&alist, 5);

    al_add_edge(alist.list[0], alist.list[1], 1);
    al_add_edge(alist.list[0], alist.list[3], 1);
    al_add_edge(alist.list[1], alist.list[2], 1);
    al_add_edge(alist.list[1], alist.list[3], 1);
    al_add_edge(alist.list[2], alist.list[3], 1);
    al_add_edge(alist.list[2], alist.list[4], 1);
    al_add_edge(alist.list[3], alist.list[4], 1);

    size_t distances[256];
    bool visited[256];

    al_bfs(&alist, 1, distances, visited);

    for(int i = 0; i < 5; i++) {
        printf("test_al_bfs(): distance %d: %ld\n", i, distances[i]);
    }

    assert(distances[0] == 0);
    assert(distances[1] == 1);
    assert(distances[2] == 2);
    assert(distances[3] == 1);
    assert(distances[4] == 2);

    al_free(&alist);    
}

void test_al_dijkstra() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);
    al_add_vertex(&alist, 3);
    al_add_vertex(&alist, 4);
    al_add_vertex(&alist, 5);

    al_add_edge(alist.list[0], alist.list[1], 3);
    al_add_edge(alist.list[0], alist.list[3], 7);
    al_add_edge(alist.list[1], alist.list[2], 1);
    al_add_edge(alist.list[1], alist.list[3], 2);
    al_add_edge(alist.list[2], alist.list[3], 3);
    al_add_edge(alist.list[2], alist.list[4], 4);
    al_add_edge(alist.list[3], alist.list[4], 1);

    size_t distances[256];
    bool visited[256];

    al_dijkstra(&alist, 1, distances, visited);

    for(int i = 0; i < 5; i++) {
        printf("test_al_dijkstra(): distance %d: %ld\n", i, distances[i]);
    }

    assert(distances[0] == 0);
    assert(distances[1] == 3);
    assert(distances[2] == 4);
    assert(distances[3] == 5);
    assert(distances[4] == 6);

    al_free(&alist);    
}

void test_al_add_vertex() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);

    assert(alist.size == 2);
    assert(alist.list[0]->value == 1);
    assert(alist.list[1]->value == 2);

    al_free(&alist);
}

void test_al_add_edge() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);

    al_add_edge(alist.list[0], alist.list[1], 10);

    assert(alist.list[0]->num_neighbors == 1);
    assert(alist.list[1]->num_neighbors == 1);
    assert(alist.list[0]->neighbors[0] == alist.list[1]);
    assert(alist.list[1]->neighbors[0] == alist.list[0]);

    al_free(&alist);
}

void test_al_get_weight() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);

    al_add_edge(alist.list[0], alist.list[1], 10);

    assert(al_get_weight(alist.list[0], alist.list[1]) == 10);

    al_free(&alist);
}

void test_al_contains() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);

    assert(al_contains(&alist, alist.list[0]));
    assert(al_contains(&alist, alist.list[1]));

    al_free(&alist);
}

void test_al_get_edge_count() {
    adjacencyList alist;
    al_init(&alist);

    al_add_vertex(&alist, 1);
    al_add_vertex(&alist, 2);

    al_add_edge(alist.list[0], alist.list[1], 10);

    assert(al_get_edge_count(&alist) == 1);

    al_free(&alist);
}

void test() {
    test_al_add_vertex();
    test_al_add_edge();
    test_al_get_weight();
    test_al_contains();
    test_al_get_edge_count();
    test_al_dijkstra();
    test_al_bfs();
    printf("All tests passed!\n");
}
