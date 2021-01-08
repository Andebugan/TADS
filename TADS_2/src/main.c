#include <stdio.h>
#include "status_codes.h"
#include "country.h"

int main(void)
{
    int status = SUCCESS;

    FILE *file = fopen("in.txt", "rt");

    country_t country;
    status = ctr_scanf(file, &country);

    if (status == SUCCESS)
    {
        printf("%s\n%s\n", country.name, country.capital);
    }

    fclose(file);

    return status;
}
