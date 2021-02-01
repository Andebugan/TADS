#ifndef _WORKER_DEF_H_
#define _WORKER_DEF_H_

#include "queue.h"

// Тип системы (реализация очередей)
typedef enum
{
    NONE,
    WORKER_ARRAY,
    WORKER_LIST
} worker_type_t;

// Система с очередями на массивах
typedef struct
{
    qu_arr_t qu_1;
    qu_arr_t qu_2;
} worker_arr_t;

// Система с очередями на списках
typedef struct
{
    qu_lst_t qu_1;
    qu_lst_t qu_2;
} worker_lst_t;

#endif // _WORKER_DEF_H_
