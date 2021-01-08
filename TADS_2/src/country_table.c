#include <stdio.h>
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
