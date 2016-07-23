#include <time.h>

typedef struct {
    clock_t start;
    clock_t stop;
} stopwatch_t;

stopwatch_t stopwatch_start();
void stopwatch_stop(stopwatch_t* timer);
void stopwatch_reset(stopwatch_t* timer);
clock_t stopwatch_seconds(stopwatch_t timer);
clock_t stopwatch_millis(stopwatch_t timer);
clock_t stopwatch_nanos(stopwatch_t timer);
