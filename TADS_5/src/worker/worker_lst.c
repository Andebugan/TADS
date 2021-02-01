#include <stdlib.h>
#include "worker/worker_lst.h"

// Создание очереди-списка
worker_lst_t wl_create(void)
{
    worker_lst_t worker;

    worker.qu_1 = qul_create();
    worker.qu_2 = qul_create();

    return worker;
}

// Удаление очереди списка
void wl_destroy(worker_lst_t *worker)
{
    if (worker->qu_1 != NULL)
    {
        qul_destroy(worker->qu_1);
        qul_destroy(worker->qu_2);

        worker->qu_1 = NULL;
        worker->qu_2 = NULL;
    }
}
