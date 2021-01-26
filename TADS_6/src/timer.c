#include "timer.h"

// Функция для замера времени процессора
uint64_t proc_tick(void)
{
    uint32_t high;
    uint32_t low;
    __asm__ __volatile__(
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t proc_ticks = ((uint64_t)high << 32) | low;
    return proc_ticks;
}