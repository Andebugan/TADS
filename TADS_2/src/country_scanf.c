#include <ctype.h>
#include <stdlib.h>

#include "status_codes.h"
#include "country_table.h"
#include "country.h"
#include "assert.h"

// Разделительный и концевой символы для ввода
#define SCANF_DELIM ','
#define SCANF_END '\n'

#define SAFE_SCANF(func)   \
    if (status == SUCCESS) \
        status = func

// Формат считывания: <общие поля>, <специальные поля>
int ctr_scanf(FILE *file, country_t *country);

// Общие поля: название, кол-во жителей, столица, материк, вид туризма
static int __scanf_common(FILE *file, country_t *country);

// Поля экскурсионного туризма: кол-во объектов, основной вид
static int __scanf_sightseeing(FILE *file, country_t *country);

// Поля пляжного туризма: сезон, температура воздуха, температура воды, время полёта
static int __scanf_beach(FILE *file, country_t *country);

// Поля спортивного туризма: вид спорта, минимальная стоимость отдыха
static int __scanf_sport(FILE *file, country_t *country);

// Считывает название материка
static int __scanf_continent(FILE *file, continent_t *continent);

// Считывает вид туризма
static int __scanf_tourism_spec(FILE *file, tourism_t *tourism);

// Считывает вид экскурсионного туризма
static int __scanf_sightseeing_type(FILE *file, sightseeing_t *sightseeing);

// Считывает сезон для пляжного туризма
static int __scanf_season(FILE *file, season_t *season);

// Считывает вид спорта
static int __scanf_sport_type(FILE *file, sport_t *sport);

// Пропуск переданного символа и всех пробельных символов перед ним
static int __scanf_sym(FILE *file, int symbol);

// Считывает целое неотрицательное число
static int __scanf_uint(FILE *file, unsigned int *number);

// Считывает вещественное число
static int __scanf_float(FILE *file, float *number);

// Считывает строку до разделительного символа, обрезая пробелы по краям
static int __scanf_string(FILE *file, char *buf, int buf_size);

// Считывание поля данных
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

// Ввод общих параметров поля данных
static int __scanf_common(FILE *file, country_t *country)
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

// Ввод экскурсионного поля данных
static int __scanf_sightseeing(FILE *file, country_t *country)
{
    int status = SUCCESS;

    SAFE_SCANF(__scanf_uint(file, &country->tourism.sightseeing.objects_amount));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_sightseeing_type(file, &country->tourism.sightseeing.type));

    return status;
}

// Ввод пляжного поля данных
static int __scanf_beach(FILE *file, country_t *country)
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

// Ввод спортивного поля данных
static int __scanf_sport(FILE *file, country_t *country)
{
    int status = SUCCESS;

    SAFE_SCANF(__scanf_sport_type(file, &country->tourism.sport.type));
    SAFE_SCANF(__scanf_sym(file, SCANF_DELIM));
    SAFE_SCANF(__scanf_uint(file, &country->tourism.sport.min_cost));

    return status;
}

static int __scanf_continent(FILE *file, continent_t *continent)
{
    return __scanf_uint(file, continent);
}

static int __scanf_tourism_spec(FILE *file, tourism_t *tourism)
{
    return __scanf_uint(file, tourism);
}

static int __scanf_sightseeing_type(FILE *file, sightseeing_t *sightseeing)
{
    return __scanf_uint(file, sightseeing);
}

static int __scanf_season(FILE *file, season_t *season)
{
    return __scanf_uint(file, season);
}

static int __scanf_sport_type(FILE *file, sport_t *sport)
{
    return __scanf_uint(file, sport);
}

// Пропуск переданного символа и всех пробельных символов перед ним
static int __scanf_sym(FILE *file, int symbol)
{
    char c;

    do
    {
        c = fgetc(file);
    } while (isblank(c) && c != symbol);

    return c == symbol ? SUCCESS : SCANF_BAD_DELIM;
}

// Считывание целого неотрицательного числа
static int __scanf_uint(FILE *file, unsigned int *number)
{
    return fscanf(file, "%u", number) == 1 ? SUCCESS : SCANF_BAD_UINT;
}

// Считывание числа с плавающей точкой
static int __scanf_float(FILE *file, float *number)
{
    return fscanf(file, "%f", number) == 1 ? SUCCESS : SCANF_BAD_FLOAT;
}

// Считывание строки
static int __scanf_string(FILE *file, char *buf, int buf_size)
{
    char c;

    // Пропуск пробелов
    do
    {
        c = fgetc(file);
    } while (isblank(c) && c != SCANF_DELIM && c != SCANF_END);

    // Проверка на правильность символа
    if (c == EOF || c == SCANF_DELIM || c == SCANF_END)
        return SCANF_EMPTY_STRING;

    // Считывание символа в буфер
    int i = 0;
    do
    {
        buf[i] = c;
        c = fgetc(file);
        i++;
    } while (i + 1 < buf_size && c != SCANF_DELIM && c != SCANF_END);

    // Возвращение в файл символа-разделителя
    if (c == SCANF_DELIM || c == SCANF_END)
        ungetc(c, file);

    // Разделитель в конец буфера
    buf[i] = '\0';
    while (isblank(buf[i - 1]))
        buf[--i] = '\0';

    return SUCCESS;
}
