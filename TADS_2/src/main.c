#include <stdio.h>
#include "status_codes.h"
#include "country_table.h"

int main(void)
{
    int status = SUCCESS;

    status = ctrt_load_from_file("in.txt");
    printf("load status = %d\n", status);
    printf("table size = %d\n", country_table_size);

    return status;
}
