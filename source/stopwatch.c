#include <time.h>

#include "stopwatch.h"

stopwatch_t stopwatch_start()
{
    stopwatch_t out;
    out.start = clock();
    out.stop = 0;

    return out;
}

void stopwatch_stop(stopwatch_t* timer)
{
    timer->stop = clock();
}

void stopwatch_reset(stopwatch_t* timer)
{
    timer->start = 0;
    timer->stop = 0;
}

clock_t stopwatch_millis(stopwatch_t timer)
{
    clock_t diff = stopwatch_nanos(timer);

    return diff / 1000;
}

clock_t stopwatch_seconds(stopwatch_t timer)
{
    clock_t diff = stopwatch_millis(timer);

    return diff / 1000;
}

clock_t stopwatch_nanos(stopwatch_t timer)
{
    if (timer.stop == 0) stopwatch_stop(&timer);

    clock_t diff = timer.stop - timer.start;
 
    diff /= CLOCKS_PER_SEC/1000000;

    return diff;
}

