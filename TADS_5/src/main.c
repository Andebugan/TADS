#include <stdio.h>
#include "queue_arr.h"

int main(void)
{
    qu_arr_t queue = qua_create(10);

    for (int i = 0; i < 10; i++)
        printf("%d ", qua_at(queue, i)->index);
    printf("\n");

    qua_push_front(queue, (qdata_t){ .index = 1 });
    qua_push_front(queue, (qdata_t){ .index = 2 });
    qua_push_front(queue, (qdata_t){ .index = 3 });

    qdata_t data;
    qua_pop_back(queue, &data);
    printf("data: %d\n", data.index);

    for (int i = 0; i < 10; i++)
        printf("%d ", qua_at(queue, i)->index);
    printf("\n");

    qua_destroy(queue);
    return 0;
}
