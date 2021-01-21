#ifndef _WORKER_STATS_H_
#define _WORKER_STATS_H_

#include <stdbool.h>
#include "queue/queue_common.h"

// статистика одной очереди
typedef struct
{
    unsigned int curr_length; // текущая длина очереди
    float mean_length;        // средняя длина очереди
    float mean_time;          // среднее время пребывания заявок в очереди
} queue_stats_t;

// статистика выдаваемая при моделировании
typedef struct
{
    // статистика очередей в отдельности
    queue_stats_t qu_1;
    queue_stats_t qu_2;

    // текущие обрабатываемые заявки
    qdata_t data_1;
    qdata_t data_2;
    bool data_1_processing; // обрабатывается ли заявка первым ОА
    bool data_2_processing; // обрабатывается ли заявка вторым ОА

    // общие параметры
    float model_time;         // полное время моделирования
    float free_time;          // время простоя второго ОА
    unsigned int activations; // кол-во срабатываний первого ОА
} worker_stats_t;

#endif // _WORKER_STATS_T_
