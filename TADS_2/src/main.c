#include <stdio.h>
#include "status_codes.h"
#include "country_table.h"
#include "menu.h"

int main(void)
{
    int status = SUCCESS;

    status = ctrt_load_from_file("data.txt");
    if (status == SUCCESS)
        status = run_main_menu();

    if (status != SUCCESS)
        printf("status was not ok: %d\n", status);

    return status;
}
