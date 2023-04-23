#ifdef UNIT_TESTING
#include "../test/test.h"
#endif

#include "../include/adjacencyList.h"
#include "../include/parser.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void print_path(adjacencyList* alist, cityInfo* ci, size_t* parent, size_t index1, size_t index2, const char* city1, const char* city2) {
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
        char* city = ci_get_city(ci, city_node->value);

        if(!city)
            printf("%lu ", city_node->value);
        else
            printf("%s ", city);
    }
    printf("\n");
}

void distance_between(adjacencyList* alist, cityInfo* ci, char* city1, char* city2, void (*pathfinder)(adjacencyList*, size_t, size_t*, bool*, size_t*)) {
    size_t id1 = ci_get_index(ci, city1);
    size_t id2 = ci_get_index(ci, city2);

    size_t index1 = SIZE_MAX;
    size_t index2 = SIZE_MAX;

    for (size_t i = 0; i < alist->size; i++) {
        if (alist->list[i]->value == id1) {
            index1 = i;
        }
        if (alist->list[i]->value == id2) {
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

    pathfinder(alist, id1, distances, visited, parent);

    if (visited[index2]) {
        printf("The distance between %s and %s is %lu.\n", city1, city2, distances[index2]);
        print_path(alist, ci, parent, index1, index2, city1, city2);
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

    cityInfo ci;
    ci_init(&ci, 64, 256);

    make_vertices(&alist, &ci, "data/RomaniaVertices.txt");
    make_edges(&alist, &ci, "data/RomaniaEdges.txt");

    distance_between(&alist, &ci, "Arad", "Sibiu", al_dijkstra);
    distance_between(&alist, &ci, "Arad", "Craiova", al_dijkstra);
    distance_between(&alist, &ci, "Arad", "Bucharest", al_dijkstra);

    return 0;
}
