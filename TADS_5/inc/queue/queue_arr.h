#ifndef _QUEUE_ARR_H_
#define _QUEUE_ARR_H_

#include "queue_common.h"

// очередь на массиве
typedef struct __queue_arr *qu_arr_t;

qu_arr_t qua_create(unsigned int size);
void qua_destroy(qu_arr_t queue); // WARNING: хорошо бы по указателю

int qua_push_front(qu_arr_t queue, qdata_t data);
int qua_pop_back(qu_arr_t queue, qdata_t *data);

unsigned int qua_get_size(qu_arr_t queue);

// для непосредственного доступа (для специфических нужд)
// index == 0 для элемента back (первого пришедшего)
qdata_t *qua_at(qu_arr_t queue, unsigned int index);

#endif // _QUEUE_ARR_H_
