#ifndef _QUEUE_LST_H_
#define _QUEUE_LST_H_

#include "queue_common.h"

// Очередь на списке
typedef struct __queue_lst *qu_lst_t;

// Создание и удаление очереди по списку
qu_lst_t qul_create(void);
void qul_destroy(qu_lst_t queue);

// Добавление и удаление элемента очереди
int qul_push_front(qu_lst_t queue, qdata_t data);
int qul_pop_back(qu_lst_t queue, qdata_t *data);

// Получение размера очереди
unsigned int qul_get_size(qu_lst_t queue);

// Для непосредственного доступа (для специфических нужд)
// index == 0 для элемента back (первого пришедшего)
qdata_t *qul_at(qu_lst_t queue, unsigned int index);

#endif // _QUEUE_LST_H_
