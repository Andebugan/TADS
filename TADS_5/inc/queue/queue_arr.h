#ifndef _QUEUE_ARR_H_
#define _QUEUE_ARR_H_

#include "queue_common.h"

// Очередь на массиве
typedef struct __queue_arr *qu_arr_t;

// Создание очереди-массива
qu_arr_t qua_create(unsigned int size);
// Удаление очереди-массива
void qua_destroy(qu_arr_t queue);

// Добавление и извлечение элементов
int qua_push_front(qu_arr_t queue, qdata_t data);
int qua_pop_back(qu_arr_t queue, qdata_t *data);

// Получение размера очереди
unsigned int qua_get_size(qu_arr_t queue);

// Для непосредственного доступа (для специфических нужд)
// index == 0 для элемента back (первого пришедшего)
qdata_t *qua_at(qu_arr_t queue, unsigned int index);

#endif // _QUEUE_ARR_H_
