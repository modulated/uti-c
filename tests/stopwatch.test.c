#include "tap.h"
#include "stopwatch.h"

void test_stopwatch_start()
{
    stopwatch_t test_timer = stopwatch_start();

    ok(test_timer.start <= clock(), "stopwatch_start.");
}

void test_stopwatch_stop()
{
    stopwatch_t test_timer = stopwatch_start();
    stopwatch_stop(&test_timer);
    ok(test_timer.stop <= clock(), "stopwatch_stop.");
}

void test_stopwatch_seconds()
{
    stopwatch_t test_timer = stopwatch_start();
    clock_t time = stopwatch_seconds(test_timer);
    diag("seconds: %ld", time);
    ok(time < 1, "stopwatch_seconds.");
}

void test_stopwatch_millis()
{
    stopwatch_t test_timer = stopwatch_start();
    clock_t time = stopwatch_millis(test_timer);
    diag("millis: %ld", time);
    ok(time < 100, "stopwatch_millis.");
}

void test_stopwatch_nanos()
{
    stopwatch_t test_timer = stopwatch_start();
    clock_t time = stopwatch_nanos(test_timer);
    diag("nanos: %ld", time);
    ok(time < 1000, "stopwatch_nanos.");
}

void test_stopwatch_reset()
{
    stopwatch_t test_timer = stopwatch_start();
    stopwatch_stop(&test_timer);
    stopwatch_reset(&test_timer);
    ok(
            test_timer.start == 0 &&
            test_timer.stop == 0,
            "stopwatch_reset.");
}

void test_all()
{
    test_stopwatch_start();
    test_stopwatch_stop();
    test_stopwatch_nanos();
    test_stopwatch_millis();
    test_stopwatch_seconds();
    test_stopwatch_reset();
}

int main()
{
    plan();
    test_all();

    return exit_status();
}
