#include "../include/priorityQueue.h"

void pq_init(priorityQueue* pq, size_t capacity) {
    if(!pq)
        pq = (priorityQueue*)malloc(sizeof(priorityQueue));
    
    pq->size = 0;
    pq->capacity = capacity;
    pq->nodes = (size_t*)malloc(capacity * sizeof(size_t));
    pq->priorities = (size_t*)malloc(capacity * sizeof(size_t));
}

void pq_free(priorityQueue* pq) {
    if(!pq) return;

    free(pq->nodes);
    free(pq->priorities);
}

void pq_swap(priorityQueue *pq, size_t i, size_t j) {
    size_t temp_node = pq->nodes[i];
    size_t temp_priority = pq->priorities[i];
    pq->nodes[i] = pq->nodes[j];
    pq->priorities[i] = pq->priorities[j];
    pq->nodes[j] = temp_node;
    pq->priorities[j] = temp_priority;
}

void pq_insert(priorityQueue *pq, size_t node, size_t priority) {
    if (pq->size == pq->capacity) {
        return;
    }

    size_t i = pq->size++;
    pq->nodes[i] = node;
    pq->priorities[i] = priority;

    while(i > 0 && pq->priorities[i] < pq->priorities[(i - 1) / 2]) {
        pq_swap(pq, i, (i - 1)/2);
        i = (i - 1) / 2;
    }
}

size_t pq_extract_min(priorityQueue* pq) {
    if (pq->size == 0) {
        return SIZE_MAX;  // The priority queue is empty.
    }

    size_t min_node = pq->nodes[0];
    pq_swap(pq, 0, --pq->size);

    // Sift down.
    size_t i = 0;
    while (2 * i + 1 < pq->size) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t min_child = (right < pq->size && pq->priorities[right] < pq->priorities[left]) ? right : left;
        if (pq->priorities[min_child] >= pq->priorities[i]) {
            break;
        }
        pq_swap(pq, i, min_child);
        i = min_child;
    }

    return min_node;
}

void pq_decrease_priority(priorityQueue *pq, size_t node, size_t new_priority) {
    size_t i = SIZE_MAX;
    for(size_t j = 0; j < pq->size; ++j) {
        if(pq->nodes[j] == node) {
            i = j;
            break;
        }
    }

    if(i == SIZE_MAX)
        return;

    pq->priorities[i] = new_priority;

    while (i > 0 && pq->priorities[i] < pq->priorities[(i - 1) / 2]) {
        pq_swap(pq, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}


