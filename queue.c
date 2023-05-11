#include "queue.h"

#include <assert.h>
#include <malloc.h>

/*
 * This function initializes a queue.
 */

queue_t* queue_init() {
    queue_t* queue = malloc(sizeof(queue_t));
    assert(queue);
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

/*
 * This function adds a new node to the queue.
 */

void queue_push(queue_t* queue, void* data) {
    assert(queue);
    assert(data);
    queue_node_t* node = malloc(sizeof(queue_node_t));
    assert(node);
    node->data = data;
    node->next = NULL;
    if (queue->size == 0) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
} /* queue_push() */

/*
 * This function returns the data of the nth node in the queue.
 */

void* queue_get(queue_t* queue, uint n) {
    assert(queue);
    assert(n >= 0);
    assert(n < queue->size);
    if (n == queue->size - 1) {
        return queue->tail->data;
    }
    queue_node_t* node = queue->head;
    for (int i = 0; i < n; i++) {
        node = node->next;
    }
    return node->data;
} /* queue_get() */

/*
 * This function removes the nth node from the queue and returns its data.
 */

void* queue_pop(queue_t* queue, uint n) {
    assert(queue);
    assert(n >= 0);
    assert(n < queue->size);
    queue_node_t* node = queue->head;
    if (n == 0) {
        queue->head = node->next;
        if (--queue->size == 0) {
            queue->tail = NULL;
        }
    } else {
        for (int i = 0; i < n - 1; i++) {
            node = node->next;
        }
        queue_node_t* temp = node->next;
        node->next = temp->next;
        if (n == --queue->size) {
            queue->tail = node;
        }
        node = temp;
    }
    void* data = node->data;
    free(node);
    return data;
} /* queue_pop() */

/*
 * This function frees the queue and all of its nodes.
 */

void queue_destroy(queue_t* queue) {
    assert(queue);
    while (queue->size > 0) {
        queue_pop(queue, 0);
    }
    free(queue);
} /* queue_destroy() */


