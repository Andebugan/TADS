#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "queue/queue_arr.h"

#define QUEUE_CHECK assert(queue != NULL && "bad queue")

struct __queue_arr
{
    unsigned int size;
    unsigned int capacity;

    unsigned int front_index;
    unsigned int back_index;

    qdata_t *data;
};

qu_arr_t qua_create(unsigned int size)
{
    qu_arr_t queue = calloc(1, sizeof(struct __queue_arr));
    assert(queue && "bad array queue allocation");

    queue->data = malloc(size * sizeof(qdata_t));
    if (queue->data != NULL)
        queue->capacity = size;

    return queue;
}

void qua_destroy(qu_arr_t queue)
{
    QUEUE_CHECK;

    if (queue->data != NULL)
    {
        free(queue->data);
        queue->data = NULL;
        free(queue);
    }
}

int qua_push_front(qu_arr_t queue, qdata_t data)
{
    QUEUE_CHECK;

    int status = QUEUE_OVERFLOW;
    if (queue->size < queue->capacity)
    {
        queue->data[queue->front_index] = data;
        queue->size++;
        queue->front_index = (queue->front_index + 1) % queue->capacity;
        status = SUCCESS;
    }

    return status;
}

int qua_pop_back(qu_arr_t queue, qdata_t *data)
{
    QUEUE_CHECK;

    int status = QUEUE_EMPTY;
    if (queue->size > 0)
    {
        *data = queue->data[queue->back_index];
        queue->size--;
        queue->back_index = (queue->back_index + 1) % queue->capacity;
        status = SUCCESS;
    }

    return status;
}

unsigned int qua_get_size(qu_arr_t queue)
{
    QUEUE_CHECK;

    return queue->size;
}

qdata_t *qua_at(qu_arr_t queue, unsigned int index)
{
    QUEUE_CHECK;

    assert(index < queue->size && "bad index");
    return queue->data + (index + queue->back_index) % queue->capacity;
}
