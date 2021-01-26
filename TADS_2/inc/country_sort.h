#ifndef _COUNTRY_SORT_H_
#define _COUNTRY_SORT_H_

#include "country_table.h"

// Сортировка происходит по невариантному полю: население
typedef unsigned long long (*sort_fn_t)(void);
typedef void (*shuffle_fn_t)(void);

// Разупорядочивание таблицы
void ctrt_shuffle_table(void);

// Разупорядочивание таблицы ключей
void ctrt_shuffle_key(void);

// Все алгоритмы сортировки возвращают число микросекунд работы
// Быстрая сортировка: qsort
// Медленная сортировка: bubble sort (пузырёк)

// Быстрая сортировка исходной таблицы
unsigned long long ctrt_sort_fast_table(void);

// Быстрая сортировка таблицы ключей
unsigned long long ctrt_sort_fast_keys(void);

// Медленная сортировка исходной таблицы
unsigned long long ctrt_sort_slow_table(void);

// Медленная сортировка таблицы ключей
unsigned long long ctrt_sort_slow_keys(void);

// Сортировка таблицы с нахождением среднего времени сортировки для заданного числа интераций
float ctrt_mid_time(unsigned int iterations, sort_fn_t sort, shuffle_fn_t shuffle);

#endif // _COUNTRY_SORT_H_
