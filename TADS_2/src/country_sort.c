#include <stdlib.h>
#include <sys/time.h>
#include "country_sort.h"
#include "colors.h"

// Макрос начала отсчёта времени
#define BEGIN_TIMER                  \
    struct timeval tv_start, tv_end; \
    gettimeofday(&tv_start, NULL)

// Макрос конца отсчёта времени
#define END_TIMER                                                \
    gettimeofday(&tv_end, NULL);                                 \
    unsigned long long sec = tv_end.tv_sec - tv_start.tv_sec;    \
    unsigned long long usec = tv_end.tv_usec - tv_start.tv_usec; \
    return 1000000 * sec + usec

// Функция сравнения элементов по полю
static int __ctr_cmp(const void *ctr_1, const void *ctr_2);
// Функция сравнения ключей
static int __key_cmp(const void *key_1, const void *key_2);

// Перемешивание таблицы данных
void ctrt_shuffle_table(void)
{
    for (unsigned int i = 0; i + 1 < country_table_size; i++)
    {
        for (unsigned int j = i + 1; j < country_table_size; j++)
        {
            if (rand() % 2 == 0)
            {
                country_t ctr_1 = country_table[i];
                country_t ctr_2 = country_table[j];

                country_table[i] = ctr_2;
                country_table[j] = ctr_1;
            }
        }
    }
    init_key_table();
}

// Перемешивание таблицы ключей
void ctrt_shuffle_key(void)
{
    for (unsigned int i = 0; i + 1 < country_table_size; i++)
    {
        for (unsigned int j = i + 1; j < country_table_size; j++)
        {
            if (rand() % 2 == 0)
            {
                tkey_t key_1 = key_table[i];
                tkey_t key_2 = key_table[j];

                key_table[i] = key_2;
                key_table[j] = key_1;
            }
        }
    }
}

// Быстрая сортировка исходной таблицы
unsigned long long ctrt_sort_fast_table(void)
{
    BEGIN_TIMER;

    qsort(country_table, country_table_size, sizeof(country_t), __ctr_cmp);

    END_TIMER;
}

// Быстрая сортировка таблицы ключей
unsigned long long ctrt_sort_fast_keys(void)
{
    BEGIN_TIMER;

    qsort(key_table, country_table_size, sizeof(tkey_t), __key_cmp);

    END_TIMER;
}

// Медленная сортировка исходной таблицы (пузырёк)
unsigned long long ctrt_sort_slow_table(void)
{
    BEGIN_TIMER;

    for (unsigned int i = 0; i + 1 < country_table_size; i++)
    {
        for (unsigned int j = i + 1; j < country_table_size; j++)
        {
            country_t ctr_1 = country_table[i];
            country_t ctr_2 = country_table[j];

            if (ctr_1.population < ctr_2.population)
            {
                country_table[i] = ctr_2;
                country_table[j] = ctr_1;
            }
        }
    }

    END_TIMER;
}

// Медленная сортировка таблицы ключей
unsigned long long ctrt_sort_slow_keys(void)
{
    BEGIN_TIMER;

    for (unsigned int i = 0; i + 1 < country_table_size; i++)
    {
        for (unsigned int j = i + 1; j < country_table_size; j++)
        {
            tkey_t key_1 = key_table[i];
            tkey_t key_2 = key_table[j];

            if (key_1.population < key_2.population)
            {
                key_table[i] = key_2;
                key_table[j] = key_1;
            }
        }
    }

    END_TIMER;
}

// Сортировка таблицы с нахождением среднего времени сортировки для заданного числа интераций
float ctrt_mid_time(unsigned int iterations, sort_fn_t sort, shuffle_fn_t shuffle)
{
    unsigned long long time = 0;

    for (unsigned int i = 0; i < iterations; i++)
    {
        shuffle();
        time += sort();
    }

    return (float)time / iterations;
}

// Функция сравнения элементов по полю
static int __ctr_cmp(const void *ctr_1, const void *ctr_2)
{
    unsigned int pop_1 = ((country_t *)ctr_1)->population;
    unsigned int pop_2 = ((country_t *)ctr_2)->population;

    return pop_1 - pop_2;
}

// Функция сравнения ключей
static int __key_cmp(const void *key_1, const void *key_2)
{
    unsigned int pop_1 = ((tkey_t *)key_1)->population;
    unsigned int pop_2 = ((tkey_t *)key_2)->population;

    return pop_1 - pop_2;
}
