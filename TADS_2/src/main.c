#include <stdio.h>
#include "status_codes.h"
#include "country_table.h"

int main(void)
{
    int status = SUCCESS;

    status = ctrt_load_from_file("data.txt");
    if (status == SUCCESS)
    {
        ctrt_print();
    }

    return status;
}
