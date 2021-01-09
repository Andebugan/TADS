#ifndef _COUNTRY_SORT_H_
#define _COUNTRY_SORT_H_

#include "country_table.h"

// сортировка происходит по невариантному полю: население
typedef unsigned long long (*sort_fn_t)(void);
typedef void (*shuffle_fn_t)(void);

// разупорядочивание таблицы
void ctrt_shuffle_table(void);

// разупорядочивание таблицы ключей
void ctrt_shuffle_key(void);

// все алгоритмы сортировки:
//   возвращают число микросекунд работы

// быстрая сортировка: qsort
// медленная сортировка: bubble sort

// быстрая сортировка исходной таблицы
unsigned long long ctrt_sort_fast_table(void);

// быстрая сортировка таблицы ключей
unsigned long long ctrt_sort_fast_keys(void);

// медленная сортировка исходной таблицы
unsigned long long ctrt_sort_slow_table(void);

// медленная сортировка таблицы ключей
unsigned long long ctrt_sort_slow_keys(void);

// находит среднее время сортировки для заданного числа итераций
// оставляет таблицу отсортированной
float ctrt_mid_time(unsigned int iterations, sort_fn_t sort, shuffle_fn_t shuffle);

#endif // _COUNTRY_SORT_H_
