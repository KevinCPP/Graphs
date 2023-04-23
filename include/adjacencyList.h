#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct adjacencylist_node {
    //will store the value of the vertex
    size_t value;
    
    //will store the number of neighbors
    size_t num_neighbors;

    //will store the current capacity of the list (how much memory has been allocated)
    size_t capacity;

    //array of pointers to all neighboring nodes
    struct adjacencylist_node** neighbors;

    //array of weights to neighboring nodes. neighbors[i] has a weight of weights[i]
    size_t* weights;

} al_node;

typedef struct adjacencylist {
    //stores the actual list of nodes
    al_node** list;
    
    //stores the current capacity of the list (how much memory has been allocated)
    size_t capacity;

    //stores the real size of the list (how many nodes are on it)
    size_t size;
} adjacencyList;

// prim's algorithm
void al_prim(adjacencyList* alist, size_t start_value, size_t* parents, bool* visited);

// bfs algorithm
void al_bfs(adjacencyList* alist, size_t start_value, size_t* distances, bool* visited, size_t* parent);

// dijkstra's algorithm
void al_dijkstra(adjacencyList* alist, size_t start_value, size_t* distances, bool* visited);

// initializes an alist
void al_init(adjacencyList* alist);

// frees an alist
void al_free(adjacencyList* alist);

// search for vertices by value
al_node* al_find_vertex_by_value(adjacencyList* alist, size_t value);

//adds a vertex to an adjacency list
void al_add_vertex(adjacencyList* alist, size_t value);

//adds an edge between vertex1 and vertex2, with weight
void al_add_edge(al_node* vertex1, al_node* vertex2, size_t weight);

//gets the weight between vertex1 and vertex2
size_t al_get_weight(const al_node* vertex1, const al_node* vertex2);

//returns the total number of vertices in list
size_t al_get_vertex_count(const adjacencyList* alist);

//returns the total number of edges in list
size_t al_get_edge_count(const adjacencyList* alist);

//returns true if the vertex is contained in list
bool al_contains(const adjacencyList* alist, const al_node* vertex);

#endif
