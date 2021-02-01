#include <stdio.h>
#include "queue/queue_arr.h"
#include "status_codes.h"
#include "worker.h"
#include "menu.h"

int main(void)
{
   int status = SUCCESS;
   status = menu_loop();
   return status;
}
