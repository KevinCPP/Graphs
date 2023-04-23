#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../include/adjacencyList.h"

typedef struct cityinfo {
    size_t capacity;    // number of total hashes to be stored
    size_t buffer_size; // max string size for the cities
    char** strings;     // stores the associated strings
} cityInfo;

void ci_init(cityInfo* ci, size_t capacity, size_t buffer_size);
void ci_free(cityInfo* ci);

char* ci_get_city(cityInfo* ci, size_t index);
size_t ci_get_index(cityInfo* ci, const char* str);


void make_vertices(adjacencyList* alist, cityInfo* ci, const char* filepath);
void make_edges(adjacencyList* alist, cityInfo* ci, const char* filepath);

#endif
