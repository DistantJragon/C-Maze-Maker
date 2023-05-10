#ifndef __queue_h__
#define __queue_h__

typedef struct queue_node_s {
    void* data;
    struct queue_node_s* next;
} queue_node_t;

typedef struct queue_s {
    queue_node_t* head;
    queue_node_t* tail;
    int size;
} queue_t;

queue_t*          queue_init();
void              queue_destroy(queue_t* queue);
void*             queue_get(queue_t* queue, int n);
void              queue_push(queue_t* queue, void* data);
void*             queue_pop(queue_t* queue, int n);

#endif
