#ifndef _WORKER_LST_H_
#define _WORKER_LST_H_

#include "worker_def.h"

worker_lst_t wl_create(void);
void wl_destroy(worker_lst_t *worker);

#endif // _WORKER_LST_H_
