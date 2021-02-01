#ifndef _WORKER_ARR_H_
#define _WORKER_ARR_H_

#include "worker_def.h"

// Создание ОА
worker_arr_t wa_create(unsigned int queue_capacity);
// Удаление ОА
void wa_destroy(worker_arr_t *worker);

#endif // _WORKER_ARR_H_
