#include <stdio.h>
#include "queue/queue_arr.h"
#include "worker.h"

void print_worker_stats(void)
{
    printf("===== worker stats: =====\n");
    printf("  queue 1:\n");
    printf("    curr length: %u\n", worker_stats.qu_1.curr_length);
    printf("    mean length: %.1f\n", worker_stats.qu_1.mean_length);
    printf("    mean time: %.1f\n", worker_stats.qu_1.mean_time);
    printf("  queue 2:\n");
    printf("    curr length: %u\n", worker_stats.qu_2.curr_length);
    printf("    mean length: %.1f\n", worker_stats.qu_2.mean_length);
    printf("    mean time: %.1f\n", worker_stats.qu_2.mean_time);
    printf("  model time: %.2f\n", worker_stats.model_time);
    printf("  free time: %.2f\n", worker_stats.free_time);
    printf("  activations: %u\n", worker_stats.activations);
}

int main(void)
{
    worker_use(WORKER_ARRAY, DEFAULT_QUEUE_CAPACITY);

    print_worker_stats();

    T1 = (time_intv_t){ .min = 0.0f, .max = 6.0f };
    T2 = (time_intv_t){ .min = 1.0f, .max = 8.0f };
    probability = 0.7f;
    worker_run(100);

    print_worker_stats();

    worker_destroy();
    return 0;
}
