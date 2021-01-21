#ifndef _COUNTRY_TABLE_H_
#define _COUNTRY_TABLE_H_

#include "country.h"

#define MAX_TABLE_SIZE 128

extern country_t country_table[MAX_TABLE_SIZE];
extern unsigned int country_table_size;

typedef struct
{
    unsigned int index;
    unsigned int population;
} tkey_t;

extern tkey_t key_table[MAX_TABLE_SIZE];

// загружает таблицу из файла
int ctrt_load_from_file(const char *filename);

// выводит таблицу на экран
void ctrt_print(void);

// выводит таблицу ключей
void ctrt_print_keys(void);

// выводит таблицу по ключам
void ctrt_print_by_keys(void);

// выводит записи о странах на указанном
// материке с указанным видом спорта
void ctrt_select(continent_t continent, sport_t sport);

#endif // _COUNTRY_TABLE_H_
