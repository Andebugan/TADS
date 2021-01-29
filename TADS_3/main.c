#include "menu.h"
#include "status_codes.h"
#include "sparse_matrix.h"

int main(void)
{
    int status = SUCCESS;

    status = menu_loop();

    return status;
}
