#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "status_codes.h"
#include "country_table.h"

// Объявление таблицы
country_t country_table[MAX_TABLE_SIZE];
// Объявление размера таблицы
unsigned int country_table_size;

// Объявление таблицы ключей
tkey_t key_table[MAX_TABLE_SIZE];

// Вывод данных об одной записи
static void __print_country(country_t country);

// Инициализация таблицы ключей
void init_key_table(void);

// Загрузка таблицы из файла
int ctrt_load_from_file(const char *filename)
{
    int status = SUCCESS;
    // Открытие файла
    FILE *file = fopen(filename, "rt");
    // Обработка ошибок 
    if (file == NULL)
        status = INVALID_FILENAME;
    else
    {
        // Инициализация размера таблицы
        country_table_size = 0;
        // Заполнение таблицы
        while (status == SUCCESS && !feof(file) &&
            country_table_size < MAX_TABLE_SIZE)
        {
            status = ctr_scanf(file, country_table + country_table_size);
            country_table_size++;

            // Конец файла
            char c = fgetc(file);
            if (c != EOF)
                ungetc(c, file);
        }

        if (country_table_size == MAX_TABLE_SIZE)
            status = MAX_COUNTRY_TABLE_SIZE_REACHED;
        
        if (status == SUCCESS)
            init_key_table();

        fclose(file);
    }

    return status;
}

// Вывод таблицы на экран
void ctrt_print(void)
{
    for (unsigned int i = 0; i < country_table_size; i++)
    {
        printf("%u - ", i + 1);
        __print_country(country_table[i]);
    }
}

// Вывод таблицы ключей
void ctrt_print_keys(void)
{
    for (unsigned int i = 0; i < country_table_size; i++)
        printf("%3u | %3u | %u\n", i, key_table[i].index, key_table[i].population);
}

// Вывод таблицы по ключам
void ctrt_print_by_keys(void)
{
    for (unsigned int i = 0; i < country_table_size; i++)
    {
        printf("%u(%u) - ", i + 1, key_table[i].index + 1);
        __print_country(country_table[key_table[i].index]);
    }
}

// Выводит записи о странах на указанном материке с указанным видом спорта
void ctrt_select(continent_t continent, sport_t sport)
{
    bool founded = false;
    for (unsigned int i = 0; i < country_table_size; i++)
    {
        if (country_table[i].continent == continent &&
            country_table[i].tourism_spec == SPORT &&
            country_table[i].tourism.sport.type == sport)
        {
            __print_country(country_table[i]);
            founded = true;
        }
    }

    if (!founded)
        printf("Нет подходящих записей.\n");
}

// Вывод одной записи
static void __print_country(country_t country)
{
    printf("страна: %s, столица: %s, население: %u ч.\n",
        country.name, country.capital, country.population);
    printf("основной туризм: %s\n",
        country.tourism_spec == SIGHTSEEING ? "экскурсионный" :
        country.tourism_spec == BEACH ? "пляжный" : "спортивный");

    // Вариантные поля записей
    switch (country.tourism_spec)
    {
    // Экскурсионный тип
    case SIGHTSEEING:
        printf("  кол-во объектов: %u\n",
            country.tourism.sightseeing.objects_amount);
        printf("  вид: %s\n",
            country.tourism.sightseeing.type == NATURE ? "природа" :
            country.tourism.sightseeing.type == HISTORY ? "история" : "искусство");
        break;
    // Пляжный тип
    case BEACH:
        printf("  основной сезон: %s\n",
            country.tourism.beach.season == MAY ? "май" :
            country.tourism.beach.season == JUNE ? "июнь" : "июль");
        printf("  температура воздуха: %.1f°C\n",
            country.tourism.beach.air_temperature);
        printf("  температура воды: %.1f°C\n",
            country.tourism.beach.water_temperature);
        printf("  время полёта до страны: %.1f ч.\n",
            country.tourism.beach.travel_time);
        break;
    // Спортивный тип
    case SPORT:
        printf("  вид спорта: %s\n",
            country.tourism.sport.type == SKIING ? "горные лыжи" :
            country.tourism.sport.type == SERFING ? "серфинг" : "восхождение");
        printf("  минимальная стоимость отдыха: %u р.\n",
            country.tourism.sport.min_cost);
        break;
    // Ошибка в данных
    default:
        assert(0 && "invalid tourism species");
        break;
    }

    printf("\n");
}

// Инициализация таблицы ключей
void init_key_table(void)
{
    for (unsigned int i = 0; i < country_table_size; i++)
    {
        key_table[i].index = i;
        key_table[i].population = country_table[i].population;
    }
}
