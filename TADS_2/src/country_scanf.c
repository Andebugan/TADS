#include <ctype.h>
#include <stdlib.h>

#include "status_codes.h"
#include "country.h"
#include "assert.h"

// разделительный и концевой символы для ввода
#define SCANF_DELIM ','
#define SCANF_END ';'

#define SAFE_SCANF(func)   \
    if (status == SUCCESS) \
        status = func

// формат считывания:
// <общие поля>, <специальные поля>
int ctr_scanf(FILE *file, country_t *country);

// общие поля:
// название, кол-во жителей, столица, материк, вид туризма
int __scanf_common(FILE *file, country_t *country);

// поля экскурсионного туризма:
// кол-во объектов, основной вид
int __scanf_sightseeing(FILE *file, country_t *country);

// поля пляжного туризма:
// сезон, температура воздуха, температура воды, время полёта
int __scanf_beach(FILE *file, country_t *country);

// поля спортивного туризма:
// вид спорта, минимальная стоимость отдыха
int __scanf_sport(FILE *file, country_t *country);

// === специальные функции ===

// считывает название материка
int __scanf_continent(FILE *file, continent_t *continent);

// считывает вид туризма
int __scanf_tourism_spec(FILE *file, tourism_t *tourism);

// считывает вид экскурсионного туризма
int __scanf_sightseeing_type(FILE *file, sightseeing_t *sightseeing);

// считывает сезон для пляжного туризма
int __scanf_season(FILE *file, season_t *season);

// считывает вид спорта
int __scanf_sport_type(FILE *file, sport_t *sport);

// ===========================

// === вспомогательные функции ===

// пропускает переданный символ и все
// пробельные символы перед ним
int __scanf_sym(FILE *file, int symbol);

// считывает целое неотрицательное число
int __scanf_uint(FILE *file, unsigned int *number);

// считывает вещественное число
int __scanf_float(FILE *file, float *number);

// считывает строку до разделительного
// символа, обрезая пробелы по краям
int __scanf_string(FILE *file, char *buf, int buf_size);

// ===============================

int ctr_scanf(FILE *file, country_t *country)
{
    int status = __scanf_common(file, country);
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));

    if (status == SUCCESS)
    {
        switch (country->tourism_spec)
        {
        case SIGHTSEEING:
            status = __scanf_sightseeing(file, country);
            break;

        case BEACH:
            status = __scanf_beach(file, country);
            break;

        case SPORT:
            status = __scanf_sport(file, country);
            break;

        default:
            assert(0 && "invalid tourism species");
            break;
        }
    }

    SAFE_SCANF(__scanf_sym(file, SCANF_END));

    return status;
}

// ================================= //

int __scanf_common(FILE *file, country_t *country)
{
    int status = SUCCESS;

    SAFE_SCANF(__scanf_string(file, country->name, COUNTRY_MAX_NAME_LENGTH));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_uint(file, &country->population));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_string(file, country->capital, CAPITAL_MAX_NAME_LENGTH));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_continent(file, &country->continent));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_tourism_spec(file, &country->tourism_spec));

    return status;
}

int __scanf_sightseeing(FILE *file, country_t *country)
{
    int status = SUCCESS;

    SAFE_SCANF(__scanf_uint(file, &country->tourism.sightseeing.objects_amount));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_sightseeing_type(file, &country->tourism.sightseeing.type));

    return status;
}

int __scanf_beach(FILE *file, country_t *country)
{
    int status = SUCCESS;

    SAFE_SCANF(__scanf_season(file, &country->tourism.beach.season));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_float(file, &country->tourism.beach.air_temperature));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_float(file, &country->tourism.beach.water_temperature));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_float(file, &country->tourism.beach.travel_time));

    return status;
}

// поля спортивного туризма:
// вид спорта, минимальная стоимость отдыха
int __scanf_sport(FILE *file, country_t *country)
{
    int status = SUCCESS;

    SAFE_SCANF(__scanf_sport_type(file, &country->tourism.sport.type));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_uint(file, &country->tourism.sport.min_cost));

    return status;
}

// ================================= //

int __scanf_continent(FILE *file, continent_t *continent)
{
    // TODO: сделать ввод по названиям материков
    return __scanf_uint(file, continent);
}

int __scanf_tourism_spec(FILE *file, tourism_t *tourism)
{
    // TODO: сделать ввод по названиям
    return __scanf_uint(file, tourism);
}

int __scanf_sightseeing_type(FILE *file, sightseeing_t *sightseeing)
{
    // TODO: сделать ввод по названиям
    return __scanf_uint(file, sightseeing);
}

int __scanf_season(FILE *file, season_t *season)
{
    // TODO: сделать ввод по названиям
    return __scanf_uint(file, season);
}

int __scanf_sport_type(FILE *file, sport_t *sport)
{
    // TODO: сделать ввод по названиям
    return __scanf_uint(file, sport);
}

// ================================= //

int __scanf_sym(FILE *file, int symbol)
{
    char c;

    do
    {
        c = fgetc(file);
    } while (isblank(c) && c != symbol);

    return c == symbol ? SUCCESS : SCANF_BAD_DELIM;
}

int __scanf_uint(FILE *file, unsigned int *number)
{
    return fscanf(file, "%u", number) == 1 ? SUCCESS : SCANF_BAD_UINT;
}

int __scanf_float(FILE *file, float *number)
{
    return fscanf(file, "%f", number) == 1 ? SUCCESS : SCANF_BAD_FLOAT;
}

int __scanf_string(FILE *file, char *buf, int buf_size)
{
    char c;

    // skip blank chars
    do
    {
        c = fgetc(file);
    } while (isblank(c) && c != SCANF_DELIM && c != SCANF_END);

    // check for bad char
    if (c == EOF || c == SCANF_DELIM || c == SCANF_END)
        return SCANF_EMPTY_STRING;

    // put chars in buffer
    int i = 0;
    do
    {
        buf[i] = c;
        c = fgetc(file);
        i++;
    } while (i + 1 < buf_size && c != SCANF_DELIM && c != SCANF_END);

    // put back delim char
    if (c == SCANF_DELIM || c == SCANF_END)
        ungetc(c, file);

    // strip from the end of buffer
    buf[i] = '\0';
    while (isblank(buf[i - 1]))
        buf[--i] = '\0';

    return SUCCESS;
}
