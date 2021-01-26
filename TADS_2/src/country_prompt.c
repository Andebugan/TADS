#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include "country.h"
#include "status_codes.h"

#define SAFE_SCANF(func)   \
    if (status == SUCCESS) \
    status = func

#define SAFE_PRINTF(func)  \
    if (status == SUCCESS) \
    func

int ctr_prompt(country_t *country);

static int __prompt_common(country_t *country);
static int __prompt_sightseeing(country_t *country);
static int __prompt_beach(country_t *country);
static int __prompt_sport(country_t *country);

// === вспомогательные функции === //

static int __scanf_string(char *buf, int buf_size);
static int __scanf_option(int *option, ...);
static int __scanf_uint(unsigned int *number);
static int __scanf_float(float *number);

// =============================== //

int ctr_prompt(country_t *country)
{
    int status = __prompt_common(country);
    if (status == SUCCESS)
    {
        switch (country->tourism_spec)
        {
        case SIGHTSEEING:
            status = __prompt_sightseeing(country);
            break;

        case BEACH:
            status = __prompt_beach(country);
            break;

        case SPORT:
            status = __prompt_sport(country);
            break;

        default:
            assert(0 && "bad tourism specie");
            break;
        }
    }

    return status;
}

// ================================= //

static int __prompt_common(country_t *country)
{
    int status = SUCCESS;

    SAFE_PRINTF(printf("Введите название страны (не более %d символов):\n", COUNTRY_MAX_NAME_LENGTH - 1));
    SAFE_SCANF(__scanf_string(country->name, COUNTRY_MAX_NAME_LENGTH));

    SAFE_PRINTF(printf("Введите число жителей страны: "));
    SAFE_SCANF(__scanf_uint(&country->population));

    SAFE_PRINTF(printf("Введите название столицы страны (не более %d символов):\n", CAPITAL_MAX_NAME_LENGTH - 1));
    SAFE_SCANF(__scanf_string(country->capital, CAPITAL_MAX_NAME_LENGTH));

    SAFE_PRINTF(printf("Выберите материк, на котором располагается страна:\n"));
    SAFE_SCANF(__scanf_option(&country->continent,
        "Африка",
        "Антарктика",
        "Австралия",
        "Евразия",
        "Северная Америка",
        "Южная Америка",
        NULL
    ));

    SAFE_PRINTF(printf("Выберите основной вид туризма:\n"));
    SAFE_SCANF(__scanf_option(&country->tourism_spec,
        "Экскурсионный",
        "Пляжный",
        "Спортивный",
        NULL
    ));

    return status;
}

static int __prompt_sightseeing(country_t *country)
{
    int status = SUCCESS;

    SAFE_PRINTF(printf("Введите кол-во объектов: "));
    SAFE_SCANF(__scanf_uint(&country->tourism.sightseeing.objects_amount));

    SAFE_PRINTF(printf("Выберите основной вид:\n"));
    SAFE_SCANF(__scanf_option(&country->tourism.sightseeing.type,
        "Природа",
        "История",
        "Искусство",
        NULL
    ));

    return status;
}

static int __prompt_beach(country_t *country)
{
    int status = SUCCESS;

    SAFE_PRINTF(printf("Выберите основной сезон:\n"));
    SAFE_SCANF(__scanf_option(&country->tourism.beach.season,
        "Май",
        "Июнь",
        "Июль",
        NULL
    ));

    SAFE_PRINTF(printf("Введите температуру воздуха (°C): "));
    SAFE_SCANF(__scanf_float(&country->tourism.beach.air_temperature));

    SAFE_PRINTF(printf("Введите температуру воды (°C): "));
    SAFE_SCANF(__scanf_float(&country->tourism.beach.water_temperature));

    SAFE_PRINTF(printf("Введите время полёта до страны (ч): "));
    SAFE_SCANF(__scanf_float(&country->tourism.beach.travel_time));

    return status;
}

static int __prompt_sport(country_t *country)
{
    int status = SUCCESS;

    SAFE_PRINTF(printf("Выберите вид спорта:\n"));
    SAFE_SCANF(__scanf_option(&country->tourism.sport.type,
        "Горные лыжи",
        "Серфинг",
        "Восхождения",
        NULL
    ));

    SAFE_PRINTF(printf("Введите минимальную стоимость отдыха (р): "));
    SAFE_SCANF(__scanf_uint(&country->tourism.sport.min_cost));

    return status;
}

// ================================= //

static int __scanf_string(char *buf, int buf_size)
{
    char c;

    // skip blank chars
    do
    {
        c = getchar();
    } while (isblank(c) && c != '\n');

    // check for bad char
    if (c == EOF || c == '\n')
        return SCANF_EMPTY_STRING;

    // put chars in buffer
    int i = 0;
    do
    {
        buf[i] = c;
        c = getchar();
        i++;
    } while (i + 1 < buf_size && c != '\n');

    // strip from the end of buffer
    buf[i] = '\0';
    while (isblank(buf[i - 1]))
        buf[--i] = '\0';

    return SUCCESS;
}

static int __scanf_option(int *option, ...)
{
    int status = SCANF_BAD_UINT;
    va_list args;

    va_start(args, option);
    const char *option_string = va_arg(args, const char *);
    int index = 0;

    while (option_string != NULL)
    {
        printf("  %d. %s\n", index, option_string);
        option_string = va_arg(args, const char *);
        index++;
    }

    va_end(args);

    printf(": ");
    if (scanf("%d", option) == 1)
    {
        getchar();
        if (*option < 0 || *option >= index)
            status = SCANF_BAD_OPTION;
        else
            status = SUCCESS;
    }

    return status;
}

static int __scanf_uint(unsigned int *number)
{
    int status = SCANF_BAD_UINT;
    if (scanf("%u", number) == 1)
    {
        getchar();
        status = SUCCESS;
    }

    return status;
}

static int __scanf_float(float *number)
{
    int status = SCANF_BAD_FLOAT;
    if (scanf("%f", number) == 1)
    {
        getchar();
        status = SUCCESS;
    }

    return status;
}
