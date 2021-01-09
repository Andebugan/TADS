#include <stdio.h>
#include "status_codes.h"
#include "country_table.h"
#include "country_sort.h"

int main(void)
{
    int status = SUCCESS;

    /*
    status = ctrt_load_from_file("data.txt");
    if (status == SUCCESS)
    {
        // ctrt_print();
        float time = ctrt_mid_time(100000, ctrt_sort_fast_table, ctrt_shuffle_table);
        printf("среднее время сортировки (fast): %.2f мкс\n", time);

        time = ctrt_mid_time(100000, ctrt_sort_slow_table, ctrt_shuffle_table);
        printf("среднее время сортировки (slow): %.2f мкс\n", time);
    }
    */
    country_t ctr;
    status = ctr_prompt(&ctr);

    if (status == SUCCESS)
        printf("ok!\n");

    return status;
}
