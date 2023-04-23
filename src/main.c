#ifdef UNIT_TESTING
#include "../test/test.h"
#endif

#include "../include/adjacencyList.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


unsigned char hashIndex = 0;
size_t hashes[256];
char strings[256][256];

char* unhash(size_t hash);

// simple hashing algorithm (djb2)
size_t hash(char *str) {
    size_t hash = 5381;
    int c;

    if(!unhash(hash)) {
        strcpy(strings[hashIndex], str);
    }

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    // cache this string's hash so we can convert the hash back into a string later
    if(!unhash(hash)) {
        hashes[hashIndex] = hash;
        hashIndex += 1;
    }

    return hash;
}

// don't judge me
char* unhash(size_t hash) {
    for(int i = 0; i < hashIndex; i++) {
        if(hashes[i] == hash)
            return strings[i];
    }

    return NULL;
}

void make_vertices(adjacencyList* alist, const char* filepath) {
    // open the file with all of the city names
    FILE *fp = fopen(filepath, "r");

    if(!fp) {
        fprintf(stdout, "ERROR | main.cpp:make_vertices | failed to open file (errno: %d)\n", errno);
        exit(EXIT_FAILURE);
    }

    // read line by line, (current line will be stored in buffer)
    char buffer[256];
    while(fgets(buffer, 256 * sizeof(char), fp)) {
        
        char sanetize[256];
        for(int i = 0; i < 256; i++) {
            if(buffer[i] == '\r' || buffer[i] == '\n') {
                sanetize[i] = '\0';
                break;
            }

            sanetize[i] = buffer[i];
        }

        // add the vertex with a value equal to the string's hash.
        // we do this to represent the cities as numbers, since I
        // implemented alist with size_t as the value.
        al_add_vertex(alist, hash(sanetize));

        //printf("unhash test: %s\n", unhash(hash(sanetize)));
    }
}

void make_edges(adjacencyList* alist, const char* filepath) {
    // open the file with all of the city names
    FILE *fp = fopen(filepath, "r");

    if(!fp) {
        fprintf(stdout, "ERROR | main.cpp:make_vertices | failed to open file (errno: %d)\n", errno);
        exit(EXIT_FAILURE);
    }

    // read line by line, (current line will be stored in buffer)
    char buffer[256];
    while(fgets(buffer, 256 * sizeof(char), fp)) {
        // tokenize the string at each comma
        char token[3][256];
        char* endptr;
        size_t currentToken = 0;
        size_t offset = 0;
        for(size_t i = 0; i < 256; i++) {
            if(buffer[i] == '\0' || buffer[i] == '\n' || buffer[i] == '\r'){
                token[currentToken][i - offset] = '\0';
                break;
            }

            if(buffer[i] == ',') {
                token[currentToken][i - offset] = '\0';
                currentToken++;
                offset = i + 1;
                continue;
            }

            token[currentToken][i - offset] = buffer[i];
        }

        al_node* vertex1 = al_find_vertex_by_value(alist, hash(token[0]));
        al_node* vertex2 = al_find_vertex_by_value(alist, hash(token[1]));
        size_t weight = strtol(token[2], &endptr, 10);

        al_add_edge(vertex1, vertex2, weight);
    }
}

void print_path(adjacencyList* alist, size_t* parent, size_t index1, size_t index2, const char* city1, const char* city2) {
    if (parent[index2] == SIZE_MAX) {
        printf("No path found between %s and %s.\n", city1, city2);
        return;
    }

    size_t path[alist->size];
    size_t path_length = 0;
    size_t current = index2;

    while (current != SIZE_MAX) {
        path[path_length++] = current;
        current = parent[current];
    }

    printf("The path between %s and %s is:", city1, city2);

    for (size_t i = path_length - 1; i != SIZE_MAX; i--) {
        al_node* city_node = alist->list[path[i]];
        char* unhashed = unhash(city_node->value);

        if(!unhashed)
            printf("%lu ", city_node->value);
        else
            printf("%s ", unhash(city_node->value));
    }
    printf("\n");
}

void distance_between(adjacencyList* alist, char* city1, char* city2) {
    size_t hash1 = hash(city1);
    size_t hash2 = hash(city2);

    size_t index1 = SIZE_MAX;
    size_t index2 = SIZE_MAX;

    for (size_t i = 0; i < alist->size; i++) {
        if (alist->list[i]->value == hash1) {
            index1 = i;
        }
        if (alist->list[i]->value == hash2) {
            index2 = i;
        }
        if (index1 != SIZE_MAX && index2 != SIZE_MAX) {
            break;
        }
    }

    if (index1 == SIZE_MAX || index2 == SIZE_MAX) {
        printf("Could not find one or both cities: %s and %s.\n", city1, city2);
        return;
    }

    size_t* parent = malloc(alist->size * sizeof(size_t));
    size_t* distances = malloc(alist->size * sizeof(size_t));
    bool* visited = malloc(alist->size * sizeof(bool));

    al_bfs(alist, hash1, distances, visited, parent);

    if (visited[index2]) {
        printf("The distance between %s and %s is %lu.\n", city1, city2, distances[index2]);
        print_path(alist, parent, index1, index2, city1, city2);
    } else {
        printf("Could not find the distance between %s and %s.\n", city1, city2);
    }

    free(distances);
    free(visited);
    free(parent);
}

int main() {
    
#ifdef UNIT_TESTING
    test();
#endif

    adjacencyList alist;
    al_init(&alist);

    make_vertices(&alist, "data/RomaniaVertices.txt");
    make_edges(&alist, "data/RomaniaEdges.txt");

    distance_between(&alist, "Arad", "Sibiu");
    distance_between(&alist, "Arad", "Craiova");
    distance_between(&alist, "Arad", "Bucharest");

    return 0;
}
