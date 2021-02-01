#include <stdlib.h>
#include "worker/worker_arr.h"

// Создание очереди массива
worker_arr_t wa_create(unsigned int queue_capacity)
{
    worker_arr_t worker;

    worker.qu_1 = qua_create(queue_capacity);
    worker.qu_2 = qua_create(queue_capacity);

    return worker;
}

// Удаление очереди массива
void wa_destroy(worker_arr_t *worker)
{
    if (worker->qu_1 != NULL)
    {
        qua_destroy(worker->qu_1);
        qua_destroy(worker->qu_2);

        worker->qu_1 = NULL;
        worker->qu_2 = NULL;
    }
}
