#include "../include/parser.h"
#include <errno.h>

void ci_init(cityInfo* ci, size_t capacity, size_t buffer_size) {
    ci->capacity = capacity;
    ci->buffer_size = buffer_size;
    ci->strings = (char**)malloc(capacity * sizeof(char*));

    for(size_t i = 0; i < capacity; i++) {
        ci->strings[i] = (char*)malloc(buffer_size * sizeof(char));
    }
}

void ci_free(cityInfo* ci) {
    for(size_t i = 0; i < ci->capacity; i++) {
        free(ci->strings[i]);
    }

    free(ci->strings);
}

char* ci_get_city(cityInfo* ci, size_t index) {
    if (index >= ci->capacity) return NULL;
    if (ci == NULL) return NULL;
    
    return ci->strings[index];
}

size_t ci_get_index(cityInfo* ci, const char* str) {
    for(size_t i = 0; i < ci->capacity; i++)
        if(strcmp(str, ci->strings[i]) == 0)
            return i;

    return SIZE_MAX;
}

void make_vertices(adjacencyList* alist, cityInfo* ci, const char* filepath) {
    // open the file with all of the city names
    FILE *fp = fopen(filepath, "r");

    if(!fp) {
        fprintf(stdout, "ERROR | main.cpp:make_vertices | failed to open file (errno: %d)\n", errno);
        exit(EXIT_FAILURE);
    }
    
    // read line by line, (current line will be stored in buffer)
    
    size_t num_cities = 0;
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
        
        // add the index that the city will be stored in (in `ci`)
        // to store the city as a numeric value
        al_add_vertex(alist, num_cities);

        // copy the actual string so the number can be converted back
        // into a city name for printing later.
        strcpy(ci->strings[num_cities], sanetize);

        // increment the index
        num_cities++;
    }
}

void make_edges(adjacencyList* alist, cityInfo* ci, const char* filepath) {
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

        al_node* vertex1 = al_find_vertex_by_value(alist, ci_get_index(ci, token[0]));
        al_node* vertex2 = al_find_vertex_by_value(alist, ci_get_index(ci, token[1]));
        size_t weight = strtol(token[2], &endptr, 10);

        al_add_edge(vertex1, vertex2, weight);
    }
}
