#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// struct that will be used to store the data for the prio queue
typedef struct priorityqueue {
    size_t size;
    size_t capacity;
    size_t *nodes;
    size_t *priorities;
} priorityQueue;

// functions to manipulate prio queue
void pq_decrease_priority(priorityQueue *pq, size_t node, size_t new_priority); 
void pq_insert(priorityQueue* pq, size_t node, size_t priority);
void pq_swap(priorityQueue* pq, size_t i, size_t j);
void pq_init(priorityQueue* pq, size_t capacity);
size_t pq_extract_min(priorityQueue* pq);
void pq_free(priorityQueue* pq);

#endif
