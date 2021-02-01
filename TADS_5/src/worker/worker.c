#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "status_codes.h"
#include "worker.h"

#define EPSILON 1.0e-3f

// Глобальные переменные для чтения
worker_type_t worker_type; // Тип текущей рабочей системы
worker_arr_t worker_arr;   // Система на массивах (доступно только когда worker_type == WORKER_ARRAY)
worker_lst_t worker_lst;   // Система на списках (доступно только когда worker_type == WORKER_LIST)
worker_stats_t worker_stats; // Статистика текущей рабочей системы

// Глобальные переменные для установки
float probability; // Вероятность P: 0 <= P < 1
time_intv_t T1, T2; // Времена работы ОА1 и ОА2

// Сохраненное при инициализации значение вместимости очереди
static unsigned int saved_queue_capacity;

// Возвращает true с заданной вероятностью (от 0 до 1)
static bool get_random_state(float probability);

// Случайно выбирает время из интервала (включая границы)
static float get_random_time(time_intv_t time);

// Сбрасывает все имеющееся состояния и
// готовит для работы систему выбранного типа
int worker_use(worker_type_t type, unsigned int queue_capacity)
{
    // Проверка на тип ОА
    assert(type != NONE && "invalid worker type");

    // Очистка ОА, если он существует
    if (worker_type == WORKER_ARRAY)
        wa_destroy(&worker_arr);
    else if (worker_type == WORKER_LIST)
        wl_destroy(&worker_lst);

    // Содание нового ОА для выполнения алгоритма
    if (type == WORKER_ARRAY)
        worker_arr = wa_create(queue_capacity);
    if (type == WORKER_LIST)
        worker_lst = wl_create();

    // Определение типа ОА и размера очереди
    worker_type = type;
    saved_queue_capacity = queue_capacity;
    // Сброс системы в начальное состояние
    worker_reset();
    return SUCCESS;
}

// Сбрасывает текущую рабочую систему в
// начальное состояние (100 заявок в первой очереди)
int worker_reset(void)
{
    // Проверка на тип ОА
    assert(worker_type != NONE && "invalid program state");

    // Если реализация - массив
    if (worker_type == WORKER_ARRAY)
    {
        // Сброс очереди для первого ОА
        qua_destroy(worker_arr.qu_1);
        // Назначение очереди с новым количеством памяти для первого ОА
        worker_arr.qu_1 = qua_create(saved_queue_capacity);
        // Сброс очереди для второго ОА
        qua_destroy(worker_arr.qu_2);
        // Назначение очереди для второго ОА
        worker_arr.qu_2 = qua_create(saved_queue_capacity);

        // Добавляем 100 заявок
        for (unsigned int i = 0; i < 100; i++)
        {
            // Создаём переменную для значения заяки с типом заявки
            qdata_t data;
            // Индекс заявки
            data.index = i;
            // Время попадания в очередь
            data.arive_time = 0.0f;
            // Добавляем в очередь
            assert(qua_push_front(worker_arr.qu_1, data) == SUCCESS && "bad push");
        }
    }
    else
    // Если реализация - линейный связный список
    {
        // Сброс очередей первого и второго ОА, и создание новых соответственно
        qul_destroy(worker_lst.qu_1);
        worker_lst.qu_1 = qul_create();
        qul_destroy(worker_lst.qu_2);
        worker_lst.qu_2 = qul_create();

        // Добавляем 100 заявок
        for (unsigned int i = 0; i < 100; i++)
        {
            qdata_t data;
            data.index = i;
            data.arive_time = 0.0f;
            assert(qul_push_front(worker_lst.qu_1, data) == SUCCESS && "bad push");
        }
    }

    // Инициализируем статистические переменные
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

// Освобождение очереди
void worker_destroy(void)
{
    // Освобождение очереди-списка
    if (worker_type == WORKER_ARRAY)
        wa_destroy(&worker_arr);
    // Освобождение очереди-массива
    else if (worker_type == WORKER_LIST)
        wl_destroy(&worker_lst);

    worker_type = NONE;
}

// Запуск процесса моделирования в текущей рабочей
// системе для n заявок вышедших из второго ОА
int worker_run(unsigned int n)
{
    unsigned int requests_passed = 0;

    // Времена до ближайших событий на данный момент
    // 1. окончание обслуживания заявки в первом ОА
    // 2. окончание обслуживания заявки во втором ОА 
    float time_left_SA1 = worker_stats.data_1_processing ? get_random_time(T1) : 0.0f;
    float time_left_SA2 = worker_stats.data_2_processing ? get_random_time(T2) : 0.0f;

    // Пока количество заявок меньше чем n заявок вышедших из второго ОА
    while (requests_passed < n)
    {
        // Если первый ОА стоит
        if (!worker_stats.data_1_processing)
        {
            // Если длинна первой очереди больше 0
            if (worker_stats.qu_1.curr_length > 0)
            {
                // Удаляем заявлку из очереди соответствующего типа
                if (worker_type == WORKER_ARRAY)
                    qua_pop_back(worker_arr.qu_1, &worker_stats.data_1);
                else
                    qul_pop_back(worker_lst.qu_1, &worker_stats.data_1);

                // Время обработки первого ОА
                time_left_SA1 = get_random_time(T1);
                // Обновление параметров первой очереди
                worker_stats.qu_1.curr_length--;
                worker_stats.data_1_processing = true;
            }
        }

        // Если второй ОА стоит
        if (!worker_stats.data_2_processing)
        {
            // Длина второй очереди больше 0
            if (worker_stats.qu_2.curr_length > 0)
            {
                // Удаляем заявлку из очереди соответствующего типа
                if (worker_type == WORKER_ARRAY)
                    qua_pop_back(worker_arr.qu_2, &worker_stats.data_2);
                else
                    qul_pop_back(worker_lst.qu_2, &worker_stats.data_2);

                // Время обработки второго ОА
                time_left_SA2 = get_random_time(T2);
                // Обновление параметров второй очереди
                worker_stats.qu_1.curr_length--;
                worker_stats.data_2_processing = true;
            }
        }

        // Вычисляем, какое событие наступит раньше
        float delta_time = fminf(
            worker_stats.data_1_processing ? time_left_SA1 : time_left_SA2,
            worker_stats.data_2_processing ? time_left_SA2 : time_left_SA1
        );

        // Продвигаемся во времени вперед
        worker_stats.model_time += delta_time;
        // "Приближаем" события
        time_left_SA1 -= delta_time;
        time_left_SA2 -= delta_time;

        // Обновляем время простоя второго ОА
        if (!worker_stats.data_1_processing)
            worker_stats.free_time += delta_time;

        // Передаем заявку, если она обслужилась
        if (worker_stats.data_1_processing && time_left_SA1 < EPSILON)
        {
            // Передаем обратно в первую очередь
            if (get_random_state(probability))
            {
                // Добавляем заявку в очередь соответствующего типа
                if (worker_type == WORKER_ARRAY)
                    qua_push_front(worker_arr.qu_1, worker_stats.data_1);
                else
                    qul_push_front(worker_lst.qu_1, worker_stats.data_1);
                
                worker_stats.qu_1.curr_length++;
            }
            else // Иначе добавляем во вторую очередь
            {
                if (worker_type == WORKER_ARRAY)
                    qua_push_front(worker_arr.qu_2, worker_stats.data_1);
                else
                    qul_push_front(worker_lst.qu_2, worker_stats.data_1);

                worker_stats.qu_2.curr_length++;
            }
            // Обновление параметров второй очереди
            worker_stats.data_1_processing = false;
            worker_stats.activations++;
        }

        // Передаем заявку из второго ОА
        if (worker_stats.data_2_processing && time_left_SA2 < EPSILON)
        {
            // Вывод заявки из очереди
            if (worker_type == WORKER_ARRAY)
                qua_push_front(worker_arr.qu_1, worker_stats.data_2);
            else
                qul_push_front(worker_lst.qu_1, worker_stats.data_2);
            // Обновление параметров второй очереди
            worker_stats.qu_1.curr_length++;
            worker_stats.data_2_processing = false;
            requests_passed++;
        }

        worker_stats.qu_1.mean_length += worker_stats.qu_1.curr_length;
        worker_stats.qu_2.mean_length += worker_stats.qu_2.curr_length;

        worker_stats.qu_1.mean_time += time_left_SA1;
        worker_stats.qu_2.mean_time += time_left_SA2;
    }

    worker_stats.qu_1.mean_length /= n;
    worker_stats.qu_2.mean_length /= n;
    worker_stats.qu_1.mean_time /= n;
    worker_stats.qu_2.mean_time /= n;

    return SUCCESS;
}

// Получение 0 или 1 с вероятностью probability
static bool get_random_state(float probability)
{
    return (float)rand() <= probability * RAND_MAX;
}

// Получение рандомного времени в рамках от time.min до time.max
static float get_random_time(time_intv_t time)
{
    return time.min + (time.max - time.min) * (float)rand() / RAND_MAX;
}


