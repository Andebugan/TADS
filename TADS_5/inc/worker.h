#ifndef _WORKER_H_
#define _WORKER_H_

#include "queue.h"
#include "worker/worker_def.h"
#include "worker/worker_stats.h"
#include "worker/worker_arr.h"
#include "worker/worker_lst.h"

#define DEFAULT_QUEUE_CAPACITY 100

typedef struct
{
    float min;
    float max;
} time_intv_t;

// глобальные переменные для чтения
extern worker_type_t worker_type; // тип текущей рабочей системы
extern worker_arr_t worker_arr; // система на массивах (доступно только когда worker_type == WORKER_ARRAY)
extern worker_lst_t worker_lst; // система на списках (доступно только когда worker_type == WORKER_LIST)
extern worker_stats_t worker_stats; // статистика текущей рабочей системы

// глобальные переменные для установки
extern float probability; // вероятность P: 0 <= P < 1
extern time_intv_t T1, T2; // времена работы ОА1 и ОА2

// сбрасывает все имеющееся состояния и
// готовит для работы систему выбранного типа
int worker_use(worker_type_t type, unsigned int queue_capacity);

// просто сбрасывает текущую рабочую систему в
// начальное состояние (100 заявок в первой очереди)
int worker_reset(void);

// очищяет все ресурсы выделенные на моделирование систем
void worker_destroy(void);

// запускает процесс моделирования в текущей рабочей
// системе для n заявок вышедших из второго ОА
int worker_run(unsigned int n);

#endif // _WORKER_H_
