#ifndef _QUEUE_LST_H_
#define _QUEUE_LST_H_

#include "queue_common.h"

// очередь на списке
typedef struct __queue_lst *qu_lst_t;

qu_lst_t qul_create(void);
void qul_destroy(qu_lst_t queue); // WARNING: хорошо бы по указателю

int qul_push_front(qu_lst_t queue, qdata_t data);
int qul_pop_back(qu_lst_t queue, qdata_t *data);

unsigned int qul_get_size(qu_lst_t queue);

// для непосредственного доступа (для специфических нужд)
// index == 0 для элемента back (первого пришедшего)
qdata_t *qul_at(qu_lst_t queue, unsigned int index);

#endif // _QUEUE_LST_H_
