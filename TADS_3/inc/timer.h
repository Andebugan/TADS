#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
// Таймер - старт
#define START_TIMER                  \
    struct timeval tv_start, tv_end; \
    gettimeofday(&tv_start, NULL)
// Таймер - конец
#define END_TIMER \
    gettimeofday(&tv_end, NULL)
// Перевод в миллисекунды
#define TIMER_MCS \
    (1000000 * (tv_end.tv_sec - tv_start.tv_sec) + tv_end.tv_usec - tv_start.tv_usec)

#endif // _TIMER_H_
