#include "../include/adjacencyList.h"
#include "../include/priorityQueue.h"

#include <stdbool.h>
#include <stdint.h>
/*
typedef struct adjacencylist_node {
    //will store the value of the vertex
    size_t value;
    
    //will store the number of neighbors
    size_t num_neighbors;

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
*/

// initialize an adjacencyList
void al_init(adjacencyList* alist) {
    alist->capacity = 0;
    alist->size = 0;
    alist->list = NULL;
}

// free the memory occupied by an adjacency list
void al_free(adjacencyList* alist) {
    for(size_t i = 0; i < alist->size; i++) {
        free(alist->list[i]->neighbors);
        free(alist->list[i]->weights);
        free(alist->list[i]);
    }

    free(alist->list);
}

// find indices by value
size_t al_find_index_by_value(adjacencyList* alist, size_t value) {
    for (size_t i = 0; i < alist->size; ++i) {
        if (alist->list[i]->value == value) {
            return i;
        }
    }
    return SIZE_MAX;
}

// find vertices by value
al_node* al_find_vertex_by_value(adjacencyList* alist, size_t value) {
    for(size_t i = 0; i < alist->size; i++) {
        if(alist->list[i]->value == value) {
            return alist->list[i];
        }
    }
    return NULL;
}

//adds a vertex to an adjacency list
void al_add_vertex(adjacencyList* alist, size_t value) {
    //allocate the new node that we'll be adding to the list
    al_node* new_node = (al_node*)malloc(sizeof(al_node));

    //initialize the new node's values
    new_node->value = value;
    new_node->num_neighbors = 0;
    new_node->neighbors = NULL;
    new_node->weights = NULL;
    new_node->capacity = 0;

    //increase the amount of memory allocated for the list if it has reached capacity
    if(alist->capacity <= alist->size) {
        //if we reach max capacity, double the amount of memory allocated to the list
        //double rather than allocate one more after each insertion to cut down on realloc calls
        size_t new_capacity = (alist->capacity == 0) ? 1 : alist->capacity * 2;

        //reallocate the list
        al_node** new_list = realloc(alist->list, new_capacity * sizeof(al_node*));

        //check to ensure that the reallocate operation was successful
        if(!new_list) {
            //print an error and exit program execution if reallocate failed
            fprintf(stderr, "Error: Failed to allocate memory for adjacency list.\n");
            exit(EXIT_FAILURE);
        }
        
        //update the member variables of the alist
        alist->list = new_list;
        alist->capacity = new_capacity;
    }

    //Check to make sure allocating memory for the new node was successful
    if(!new_node) {
        //print an error and exit program execution if reallocate failed
        fprintf(stderr, "Error: Failed to allocate memory for new node in adjacency list.\n");
        exit(EXIT_FAILURE);
    }

    //finally, add it to the list and increment the size by 1
    alist->list[alist->size] = new_node;
    alist->size += 1;
}

void error(const char *msg) {
    fprintf(stderr, msg);
    exit(EXIT_FAILURE);
}

//adds an edge between vertex1 and vertex2, with weight
void al_add_edge(al_node* vertex1, al_node* vertex2, size_t weight) {
    //check to ensure that neither vertex is null
    if(vertex1 == NULL || vertex2 == NULL) {
        error("Error: attempting to add edge with a NULL vertex.\n"); 
    }

    //resize vertex1's neighbors list if needed
    if(vertex1->capacity <= vertex1->num_neighbors) {
        size_t new_capacity = (vertex1->capacity == 0) ? 1 : vertex1->capacity * 2;
        al_node** new_neighbors = realloc(vertex1->neighbors, new_capacity * sizeof(al_node*));
        size_t* new_weights = realloc(vertex1->weights, new_capacity * sizeof(al_node*));

        if(!new_neighbors || !new_weights) 
            error("Error: failed to allocate memory for neighbors/weights when adding edge.\n");

        vertex1->weights = new_weights;
        vertex1->neighbors = new_neighbors;
        vertex1->capacity = new_capacity;
    }

    //resize vertex2's neighbors list if needed
    if(vertex2->capacity <= vertex2->num_neighbors) {
        size_t new_capacity = (vertex2->capacity == 0) ? 1 : vertex2->capacity * 2;
        al_node** new_neighbors = realloc(vertex2->neighbors, new_capacity * sizeof(al_node*));
        size_t* new_weights = realloc(vertex2->weights, new_capacity * sizeof(al_node*));

        if(!new_neighbors || !new_weights)
            error("Error: failed to allocate memory for neighbors/weights when adding edge.\n");

        vertex2->weights = new_weights;
        vertex2->neighbors = new_neighbors;
        vertex2->capacity = new_capacity;
    }

    vertex1->neighbors[vertex1->num_neighbors] = vertex2;
    vertex1->weights[vertex1->num_neighbors] = weight;
    vertex1->num_neighbors += 1;

    vertex2->neighbors[vertex2->num_neighbors] = vertex1;
    vertex2->weights[vertex2->num_neighbors] = weight;
    vertex2->num_neighbors += 1;

}

//gets the weight between vertex1 and vertex2
size_t al_get_weight(const al_node* vertex1, const al_node* vertex2) {
    if(vertex1 == NULL || vertex2 == NULL)
        error("Error: attempting to get weight of an edge with a NULL vertex.\n");

    //loop through each of vertex1's neighbors. If they are neighbors, it'll be in here
    for(size_t i = 0; i < vertex1->num_neighbors; i++) {
        al_node* current_ptr = vertex1->neighbors[i];

        if(current_ptr == vertex2){
            return vertex1->weights[i];
        }
    }

    return 0;
}

//returns the total number of edges in list
size_t al_get_edge_count(const adjacencyList* alist) {
    //loop through and count # neighbors for every node
    size_t edge_count = 0;
    for(size_t i = 0; i < alist->size; i++) {
        al_node* current_ptr = alist->list[i];
        edge_count += current_ptr->num_neighbors;
    }
    
    //return edge_count/2 because it will count each edge twice
    return (edge_count /= 2);
}

//returns true if the vertex is contained in list
bool al_contains(const adjacencyList* alist, const al_node* vertex) {
    for(size_t i = 0; i < alist->size; i++) {
        al_node* current_ptr = alist->list[i];
        if(current_ptr == vertex)
            return true;
    }

    return false;
}

void al_dijkstra(adjacencyList* alist, size_t start_value, size_t* distances, bool* visited, size_t* previous) {
    if (alist == NULL || distances == NULL || previous == NULL) {
        return;
    }

    size_t num_vertices = alist->size;

    size_t start_index = al_find_index_by_value(alist, start_value);
    if(start_index == SIZE_MAX) {
        fprintf(stderr, "Error: start vertex not found in adjacency list.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the priority queue and visited array
    priorityQueue pq;
    pq_init(&pq, num_vertices);
    for (size_t i = 0; i < num_vertices; ++i) {
        distances[i] = SIZE_MAX;
        previous[i] = SIZE_MAX;
        visited[i] = false;
    }

    // Set the starting vertex's distance to 0 and add it to the queue
    distances[start_index] = 0;
    pq_insert(&pq, start_index, 0);

    // Process vertices from the queue until it's empty
    while (pq.size != 0) {
        size_t current_index = pq_extract_min(&pq);
        if (visited[current_index]) {
            continue;
        }

        visited[current_index] = true;
        al_node* current = alist->list[current_index];

        for (size_t i = 0; i < current->num_neighbors; ++i) {
            al_node* neighbor = current->neighbors[i];
            size_t weight = current->weights[i];
            size_t neighbor_index = al_find_index_by_value(alist, neighbor->value);

            if (!visited[neighbor_index]) {
                size_t alt_distance = distances[current_index] + weight;
                if (alt_distance < distances[neighbor_index]) {
                    distances[neighbor_index] = alt_distance;
                    previous[neighbor_index] = current_index;
                    pq_insert(&pq, neighbor_index, alt_distance);
                }
            }
        }
    }

    pq_free(&pq);
}

// dijkstra's (without prio queue)
void al_dijkstra_slow(adjacencyList* alist, size_t start_value, size_t* distances, bool* visited) {
    al_node* start = al_find_vertex_by_value(alist, start_value);
    if(!start) {
        fprintf(stderr, "Error: start vertex not found in adjacency list.\n");
        exit(EXIT_FAILURE);
    }

    // Find the index of the starting vertex in the adjacency list
    size_t start_index = 0;
    for (size_t i = 0; i < alist->size; ++i) {
        if (alist->list[i]->value == start_value) {
            start_index = i;
            break;
        }
    }

    for(size_t i = 0; i < alist->size; i++) {
        distances[i] = SIZE_MAX;
        visited[i] = false;
    }

    distances[start_index] = 0;

    for(size_t i = 0; i < alist->size - 1; i++) {
        size_t min_distance = SIZE_MAX;
        size_t min_index = 0;

        for(size_t j = 0; j < alist->size; j++) {
            if(!visited[j] && distances[j] < min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }

        al_node* current = alist->list[min_index];
        visited[min_index] = true;

        for(size_t j = 0; j < current->num_neighbors; j++) {
            al_node* neighbor = current->neighbors[j];
            size_t weight = current->weights[j];

            // Find the index of the neighbor vertex in the adjacency list
            size_t neighbor_index = 0;
            for (size_t k = 0; k < alist->size; ++k) {
                if (alist->list[k]->value == neighbor->value) {
                    neighbor_index = k;
                    break;
                }
            }

            if(!visited[neighbor_index] && distances[min_index] + weight < distances[neighbor_index]) {
                distances[neighbor_index] = distances[min_index] + weight;
            }
        }
    }
}



void al_bfs(adjacencyList* alist, size_t start_value, size_t* distances, bool* visited, size_t* parent) {
    al_node* start = al_find_vertex_by_value(alist, start_value);
    if (!start) {
        fprintf(stderr, "Error: start vertex not found in adjacency list.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < alist->size; i++) {
        parent[i] = SIZE_MAX;
        distances[i] = SIZE_MAX;
        visited[i] = false;
    }

    size_t start_index = 0;
    for (size_t i = 0; i < alist->size; ++i) {
        if (alist->list[i]->value == start_value) {
            start_index = i;
            break;
        }
    }

    distances[start_index] = 0;
    visited[start_index] = true;

    al_node** to_visit = malloc(alist->size * sizeof(al_node*));
    size_t to_visit_count = 1;
    size_t current_visit_index = 0;

    to_visit[0] = start;

    while (current_visit_index < to_visit_count) {
        al_node* current = to_visit[current_visit_index++];
        size_t current_index = 0;

        for (size_t i = 0; i < alist->size; ++i) {
            if (alist->list[i]->value == current->value) {
                current_index = i;
                break;
            }
        }

        for (size_t i = 0; i < current->num_neighbors; i++) {
            al_node* neighbor = current->neighbors[i];
            size_t neighbor_index = 0;

            for (size_t j = 0; j < alist->size; ++j) {
                if (alist->list[j]->value == neighbor->value) {
                    neighbor_index = j;
                    break;
                }
            }

            if (!visited[neighbor_index]) {
                distances[neighbor_index] = distances[current_index] + 1;
                parent[neighbor_index] = current_index;
                visited[neighbor_index] = true;
                to_visit[to_visit_count++] = neighbor;
            }
        }
    }

    free(to_visit);
}

// prim's algorithm
//
// Similar to Dijkstra's, but instead of updating the distances array based on the total
// distance from the starting vertex, it updates based on edge weight between the current
// vertex and neighboring vertex. The output is stored in the `parents` array, which
// indicates the parent of each vertex in the MST
void al_prim(adjacencyList* alist, size_t start_value, size_t* parents) {
    priorityQueue pq;
    pq_init(&pq, alist->size);

    size_t* key = malloc(alist->size * sizeof(size_t));
    int* in_pq = malloc(alist->size * sizeof(int));

    for (size_t i = 0; i < alist->size; i++) {
        key[i] = SIZE_MAX;
        in_pq[i] = 1;
        parents[i] = SIZE_MAX;
    }

    key[start_value] = 0;
    pq_insert(&pq, start_value, 0);

    while (pq.size != 0) {
        size_t current = pq_extract_min(&pq);
        in_pq[current] = 0;

        al_node* current_vertex = alist->list[current];

        for (size_t i = 0; i < current_vertex->num_neighbors; i++) {
            al_node* neighbor = current_vertex->neighbors[i];
            size_t neighbor_index = al_find_index_by_value(alist, neighbor->value);
            size_t weight = current_vertex->weights[i];

            if (in_pq[neighbor_index] && weight < key[neighbor_index]) {
                parents[neighbor_index] = current;
                key[neighbor_index] = weight;
                pq_decrease_priority(&pq, neighbor_index, weight);
            }
        }
    }

    free(key);
    free(in_pq);
    pq_free(&pq);
}


