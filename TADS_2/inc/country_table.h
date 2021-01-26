#ifndef _COUNTRY_TABLE_H_
#define _COUNTRY_TABLE_H_

#include "country.h"

#define MAX_TABLE_SIZE 128

// Глобальное объявление таблицы и размера таблицы
extern country_t country_table[MAX_TABLE_SIZE];
extern unsigned int country_table_size;

// Структура поля таблицы ключей
typedef struct
{
    unsigned int index;
    unsigned int population;
} tkey_t;

// Глобальное объявление таблицы ключей
extern tkey_t key_table[MAX_TABLE_SIZE];

// Загрузка таблицы из файла
int ctrt_load_from_file(const char *filename);

// Вывод таблицы на экран
void ctrt_print(void);

// Вывод таблицы ключей
void ctrt_print_keys(void);

// Вывод таблицы по ключам
void ctrt_print_by_keys(void);

// Выводит записи о странах на указанном материке с указанным видом спорта
void ctrt_select(continent_t continent, sport_t sport);

// Инициализация таблицы ключей
void init_key_table(void);

#endif // _COUNTRY_TABLE_H_
