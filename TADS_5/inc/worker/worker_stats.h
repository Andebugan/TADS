#ifndef _WORKER_STATS_H_
#define _WORKER_STATS_H_

#include <stdbool.h>
#include "queue/queue_common.h"

// Статистика одной очереди
typedef struct
{
    unsigned int curr_length; // Текущая длина очереди
    float mean_length;        // Средняя длина очереди
    float mean_time;          // Среднее время пребывания заявок в очереди
} queue_stats_t;

// Статистика выдаваемая при моделировании
typedef struct
{
    // Статистика очередей в отдельности
    queue_stats_t qu_1;
    queue_stats_t qu_2;

    // Текущие обрабатываемые заявки
    qdata_t data_1;
    qdata_t data_2;
    bool data_1_processing; // Обрабатывается ли заявка первым ОА
    bool data_2_processing; // Обрабатывается ли заявка вторым ОА

    // Общие параметры
    float model_time;         // Полное время моделирования
    float free_time;          // Время простоя второго ОА
    unsigned int activations; // Кол-во срабатываний первого ОА
} worker_stats_t;

#endif // _WORKER_STATS_T_
