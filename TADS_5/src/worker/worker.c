#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "status_codes.h"
#include "worker.h"

#define EPSILON 1.0e-3f

// глобальные переменные для чтения
worker_type_t worker_type; // тип текущей рабочей системы
worker_arr_t worker_arr;   // система на массивах (доступно только когда worker_type == WORKER_ARRAY)
worker_lst_t worker_lst;   // система на списках (доступно только когда worker_type == WORKER_LIST)
worker_stats_t worker_stats; // статистика текущей рабочей системы

// глобальные переменные для установки
float probability; // вероятность P: 0 <= P < 1
time_intv_t T1, T2; // времена работы ОА1 и ОА2

// сохраненное при инициализации значение вместимости очереди
static unsigned int saved_queue_capacity;

// === вспомогательные функции ===

// возвращает true с заданной вероятностью (от 0 до 1)
static bool get_random_state(float probability);

// случайно выбирает время из интервала (включая границы)
static float get_random_time(time_intv_t time);

// ===============================

// сбрасывает все имеющееся состояния и
// готовит для работы систему выбранного типа
int worker_use(worker_type_t type, unsigned int queue_capacity)
{
    assert(type != NONE && "invalid worker type");

    if (worker_type == WORKER_ARRAY)
        wa_destroy(&worker_arr);
    else if (worker_type == WORKER_LIST)
        wl_destroy(&worker_lst);

    if (type == WORKER_ARRAY)
        worker_arr = wa_create(queue_capacity);
    if (type == WORKER_LIST)
        worker_lst = wl_create();

    worker_type = type;
    saved_queue_capacity = queue_capacity;
    worker_reset();
    return SUCCESS;
}

// просто сбрасывает текущую рабочую систему в
// начальное состояние (100 заявок в первой очереди)
int worker_reset(void)
{
    assert(worker_type != NONE && "invalid program state");

    if (worker_type == WORKER_ARRAY)
    {
        qua_destroy(worker_arr.qu_1);
        worker_arr.qu_1 = qua_create(saved_queue_capacity);
        qua_destroy(worker_arr.qu_2);
        worker_arr.qu_2 = qua_create(saved_queue_capacity);

        // add 100 requests
        for (unsigned int i = 0; i < 100; i++)
        {
            qdata_t data;
            data.index = i;
            data.arive_time = 0.0f;
            assert(qua_push_front(worker_arr.qu_1, data) == SUCCESS && "bad push");
        }
    }
    else
    {
        qul_destroy(worker_lst.qu_1);
        worker_lst.qu_1 = qul_create();
        qul_destroy(worker_lst.qu_2);
        worker_lst.qu_2 = qul_create();

        // add 100 requests
        for (unsigned int i = 0; i < 100; i++)
        {
            qdata_t data;
            data.index = i;
            data.arive_time = 0.0f;
            assert(qul_push_front(worker_lst.qu_1, data) == SUCCESS && "bad push");
        }
    }

    // setup stats
    worker_stats.qu_1.curr_length = 100;
    worker_stats.qu_1.mean_length = 100;
    worker_stats.qu_1.mean_time = 0.0f;
    worker_stats.qu_2.curr_length = 0;
    worker_stats.qu_2.mean_length = 0;
    worker_stats.qu_2.mean_time = 0.0f;
    worker_stats.data_1_processing = false;
    worker_stats.data_2_processing = false;
    worker_stats.activations = 0;
    worker_stats.free_time = 0.0f;
    worker_stats.model_time = 0.0f;

    return SUCCESS;
}

void worker_destroy(void)
{
    if (worker_type == WORKER_ARRAY)
        wa_destroy(&worker_arr);
    else if (worker_type == WORKER_LIST)
        wl_destroy(&worker_lst);

    worker_type = NONE;
}

// запускает процесс моделирования в текущей рабочей
// системе для n заявок вышедших из второго ОА
int worker_run(unsigned int n)
{
    unsigned int requests_passed = 0;

    // времена до ближайших событий на данный момент
    // 1. окончание обслуживания заявки в первом ОА
    // 2. окончание обслуживания заявки во втором ОА 
    float time_left_SA1 = worker_stats.data_1_processing ? get_random_time(T1) : 0.0f;
    float time_left_SA2 = worker_stats.data_2_processing ? get_random_time(T2) : 0.0f;

    while (requests_passed < n)
    {
        // берем заявки на обработку
        if (!worker_stats.data_1_processing)
        {
            if (worker_stats.qu_1.curr_length > 0)
            {
                if (worker_type == WORKER_ARRAY)
                    qua_pop_back(worker_arr.qu_1, &worker_stats.data_1);
                else
                    qul_pop_back(worker_lst.qu_1, &worker_stats.data_1);

                time_left_SA1 = get_random_time(T1);

                worker_stats.qu_1.curr_length--;
                worker_stats.data_1_processing = true;
            }
        }

        if (!worker_stats.data_2_processing)
        {
            if (worker_stats.qu_2.curr_length > 0)
            {
                if (worker_type == WORKER_ARRAY)
                    qua_pop_back(worker_arr.qu_2, &worker_stats.data_2);
                else
                    qul_pop_back(worker_lst.qu_2, &worker_stats.data_2);

                time_left_SA2 = get_random_time(T2);

                worker_stats.qu_1.curr_length--;
                worker_stats.data_2_processing = true;
            }
        }

        // вычисляем, какое событие наступит раньше
        float delta_time = fminf(
            worker_stats.data_1_processing ? time_left_SA1 : time_left_SA2,
            worker_stats.data_2_processing ? time_left_SA2 : time_left_SA1
        );

        // продвигаемся во времени вперед
        worker_stats.model_time += delta_time;
        // "приближаем" события
        time_left_SA1 -= delta_time;
        time_left_SA2 -= delta_time;

        // обновляем время простоя второго ОА
        if (!worker_stats.data_1_processing)
            worker_stats.free_time += delta_time;

        // передаем заявку, если она обслужилась
        if (worker_stats.data_1_processing && time_left_SA1 < EPSILON)
        {
            // передаем обратно в первую очередь
            if (get_random_state(probability))
            {
                if (worker_type == WORKER_ARRAY)
                    qua_push_front(worker_arr.qu_1, worker_stats.data_1);
                else
                    qul_push_front(worker_lst.qu_1, worker_stats.data_1);
                
                worker_stats.qu_1.curr_length++;
            }
            else // иначе передаем во вторую
            {
                if (worker_type == WORKER_ARRAY)
                    qua_push_front(worker_arr.qu_2, worker_stats.data_1);
                else
                    qul_push_front(worker_lst.qu_2, worker_stats.data_1);

                worker_stats.qu_2.curr_length++;
            }

            worker_stats.data_1_processing = false;
            worker_stats.activations++;
        }

        // передаем заявку из второго ОА
        if (worker_stats.data_2_processing && time_left_SA2 < EPSILON)
        {
            if (worker_type == WORKER_ARRAY)
                qua_push_front(worker_arr.qu_1, worker_stats.data_2);
            else
                qul_push_front(worker_lst.qu_1, worker_stats.data_2);

            worker_stats.qu_1.curr_length++;
            worker_stats.data_2_processing = false;
            requests_passed++;
        }
    }

    return SUCCESS;
}

// ===============================

static bool get_random_state(float probability)
{
    return (float)rand() <= probability * RAND_MAX;
}

static float get_random_time(time_intv_t time)
{
    return time.min + (time.max - time.min) * (float)rand() / RAND_MAX;
}


