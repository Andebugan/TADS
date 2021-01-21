#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "queue/queue_lst.h"

#define QUEUE_CHECK assert(queue != NULL && "bad queue")

typedef struct __node
{
    struct __node *next;
    qdata_t data;
} __node_t;

struct __queue_lst
{
    unsigned int size;

    __node_t *front;
};

qu_lst_t qul_create(void)
{
    qu_lst_t queue = calloc(1, sizeof(struct __queue_lst));
    assert(queue && "bad calloc");

    return queue;
}

void qul_destroy(qu_lst_t queue)
{
    QUEUE_CHECK;

    while (queue->front)
    {
        __node_t *next = queue->front->next;
        free(queue->front);
        queue->front = next;
    }

    queue->size = 0;
    free(queue);
}

int qul_push_front(qu_lst_t queue, qdata_t data)
{
    QUEUE_CHECK;

    __node_t *next = queue->front;
    queue->front = malloc(sizeof(__node_t));
    assert(queue->front && "bad malloc");
    queue->front->data = data;
    queue->front->next = next;
    queue->size++;
    return SUCCESS;
}

int qul_pop_back(qu_lst_t queue, qdata_t *data)
{
    QUEUE_CHECK;

    int status = QUEUE_EMPTY;
    if (queue->size > 0)
    {
        __node_t **node = &queue->front;
        while ((*node)->next != NULL)
            node = &(*node)->next;
        
        *data = (*node)->data;
        free(*node);
        (*node) = NULL;
        queue->size--;
    }

    return status;
}

unsigned int qul_get_size(qu_lst_t queue)
{
    QUEUE_CHECK;

    return queue->size;
}

qdata_t *qul_at(qu_lst_t queue, unsigned int index)
{
    QUEUE_CHECK;

    assert(index < queue->size && "bad index");
    __node_t *node = queue->front;

    index = queue->size - index;
    while (--index > 0)
        node = node->next;

    return &node->data;
}
