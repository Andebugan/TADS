#include <stdio.h>
#include "queue/queue_arr.h"

int main(void)
{
    qu_arr_t queue = qua_create(10);

    qua_push_front(queue, (qdata_t){ .index = 1 });
    qua_push_front(queue, (qdata_t){ .index = 2 });
    qua_push_front(queue, (qdata_t){ .index = 3 });

    for (unsigned int i = 0; i < qua_get_size(queue); i++)
        printf("%d ", qua_at(queue, i)->index);
    printf("\n");

    qdata_t data;
    qua_pop_back(queue, &data);
    printf("data: %d\n", data.index);

    for (unsigned int i = 0; i < qua_get_size(queue); i++)
        printf("%d ", qua_at(queue, i)->index);
    printf("\n");

    qua_destroy(queue);
    return 0;
}
