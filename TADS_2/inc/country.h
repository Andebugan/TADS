#ifndef _COUNTRY_H_
#define _COUNTRY_H_

#include <stdio.h>

#define COUNTRY_MAX_NAME_LENGTH 32
#define CAPITAL_MAX_NAME_LENGTH 32

typedef enum
{
    AFRICA,
    ANTARCTICA,
    AUSTRALIA,
    EURASIA,
    NORTH_AMERICA,
    SOUTH_AMERICA
} continent_t;

typedef enum
{
    SIGHTSEEING,    // экскурсионный
    BEACH,          // пляжный
    SPORT,          // спортивный
} tourism_t;

typedef enum
{
    NATURE,
    HISTORY,
    ART
} sightseeing_t;

typedef enum
{
    MAY,
    JUNE,
    JULY
} season_t;

typedef enum
{
    SKIING,
    SERFING,
    CLIMBING
} sport_t;

// Основная структура записи таблицы
typedef struct
{
    char name[COUNTRY_MAX_NAME_LENGTH];
    char capital[CAPITAL_MAX_NAME_LENGTH];
    unsigned int population;
    continent_t continent;
    tourism_t tourism_spec;
    union
    {
        struct
        {
            unsigned int objects_amount;
            sightseeing_t type;
        } sightseeing;
        struct
        {
            season_t season;
            float air_temperature;
            float water_temperature;
            float travel_time;
        } beach;
        struct
        {
            sport_t type;
            unsigned int min_cost;
        } sport;
    } tourism;
} country_t;

int ctr_scanf(FILE *file, country_t *country);
int ctr_prompt(country_t *country);
int ctr_search(continent_t *continent, sport_t *sport);
// Удаление элемента
int delete_by_index(void);

#endif // _COUNTRY_H_
