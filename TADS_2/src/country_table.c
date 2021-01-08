#include <stdio.h>
#include <assert.h>
#include "status_codes.h"
#include "country_table.h"

country_t country_table[MAX_COUNTRY_TABLE_SIZE];
unsigned int country_table_size;

int ctrt_load_from_file(const char *filename)
{
    int status = SUCCESS;

    FILE *file = fopen(filename, "rt");
    if (file == NULL)
        status = INVALID_FILENAME;
    else
    {
        country_table_size = 0;
        while (status == SUCCESS && !feof(file) &&
            country_table_size < MAX_COUNTRY_TABLE_SIZE)
        {
            status = ctr_scanf(file, country_table + country_table_size);
            country_table_size++;

            // trigger eof
            char c = fgetc(file);
            if (c != EOF)
                ungetc(c, file);
        }

        if (country_table_size == MAX_COUNTRY_TABLE_SIZE)
            status = MAX_COUNTRY_TABLE_SIZE_REACHED;

        fclose(file);
    }

    return status;
}

void ctrt_print(void)
{
    for (unsigned int i = 0; i < country_table_size; i++)
    {
        country_t ctr = country_table[i];

        printf("страна: %s, столица: %s, население: %u ч.\n",
            ctr.name, ctr.capital, ctr.population);
        printf("основной туризм: %s\n",
            ctr.tourism_spec == SIGHTSEEING ? "экскурсионный" :
            ctr.tourism_spec == BEACH ? "пляжный" : "спортивный");

        switch (ctr.tourism_spec)
        {
        case SIGHTSEEING:
            printf("  кол-во объектов: %u\n", ctr.tourism.sightseeing.objects_amount);
            printf("  вид: %s\n", ctr.tourism.sightseeing.type == NATURE ? "природа" :
                ctr.tourism.sightseeing.type == HISTORY ? "история" : "искусство");
            break;

        case BEACH:
            printf("  основной сезон: %s\n",
                ctr.tourism.beach.season == MAY ? "май" :
                ctr.tourism.beach.season == JUNE ? "июнь" : "июль");
            printf("  температура воздуха: %.1f°C\n", ctr.tourism.beach.air_temperature);
            printf("  температура воды: %.1f°C\n", ctr.tourism.beach.water_temperature);
            printf("  время полёта до страны: %.1f ч.\n", ctr.tourism.beach.travel_time);
            break;

        case SPORT:
            printf("  вид спорта: %s\n",
                ctr.tourism.sport.type == SKIING ? "горные лыжи" :
                ctr.tourism.sport.type == SERFING ? "серфинг" : "восхождение");
            printf("  минимальная стоимость отдыха: %u р.\n",
                ctr.tourism.sport.min_cost);
            break;

        default:
            assert(0 && "invalid tourism species");
            break;
        }

        printf("\n");
    }
}
