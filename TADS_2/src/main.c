#include <stdio.h>
#include "status_codes.h"
#include "country_table.h"
#include "country_sort.h"

int main(void)
{
    int status = SUCCESS;

    status = ctrt_load_from_file("data.txt");
    if (status == SUCCESS)
    {
        float time = ctrt_mid_time(1000, ctrt_sort_slow_table, ctrt_shuffle_table);
        ctrt_print();

        printf("среднее время сортировки: %.2f мкс\n", time);
    }

    return status;
}
