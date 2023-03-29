#include "../include/adjacencyList.h"
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


//adds a vertex to an adjacency list
void al_add_vertex(adjacencyList* alist, size_t value) {
    //allocate the new node that we'll be adding to the list
    al_node* new_node = (al_node*)malloc(sizeof(al_node));

    //initialize the new node's values
    new_node->value = value;
    new_node->num_neighbors = 0;
    new_node->neighbors = NULL;
    new_node->weights = NULL;

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

//adds an edge between vertex1 and vertex2, with weight
void al_add_edge(al_node* vertex1, al_node* vertex2, size_t weight) {
        
}

//gets the weight between vertex1 and vertex2
size_t al_get_weight(const al_node* vertex1, const al_node* vertex2) {

}

//returns the total number of vertices in list
size_t al_get_vertex_count(const adjacencyList* alist) {

}

//returns the total number of edges in list
size_t al_get_edge_count(const adjacencyList* alist) {

}

//returns true if the vertex is contained in list
bool al_contains(const adjacencyList* alist, const al_node* vertex) {

}


