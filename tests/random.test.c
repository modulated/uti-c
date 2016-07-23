#include "tap.h"
#include <limits.h>
#include "random.h"

void test_run(void);

int main() {
    plan();

    test_run();
    
    return exit_status();
}

void test_random_int() {
    
    int test_int = random_int();
    ok1(test_int >= INT_MIN && test_int <= INT_MAX);
}

void test_random_char() {
    
    int test_char = random_char();
    ok1(test_char >= CHAR_MIN && test_char <= CHAR_MAX);
}

void test_random_long() {
    
    long test_long = random_long();
    ok1(test_long >= LONG_MIN && test_long <= LONG_MAX);
}

void test_random_llong() {
    
    long long test_llong = random_llong();
    ok1(test_llong >= LLONG_MIN && test_llong <= LLONG_MAX);
}

void test_prandom_seed()
{
    int test1 = prandom_int();
    prandom_seed(10);
    int test2 = prandom_int();

    ok(test1 != test2, "prandom_seed.") ||
    diag("Got %d and %d, should be different", test1, test2);
}

void test_prandom_int_range()
{
    int min = 1;
    int max = 2;

    int val = prandom_int_range(min, max);
    ok(
            val >= min &&
            val <= max,
            "prandom_int_range.") ||
    diag("Expected %d to be between %d and %d.", val, min, max);
}

void test_prandom_double_range()
{
    double min = 0.0;
    double max = 1.0;
    
    double val = prandom_double_range(min, max);
    ok(
            val >= min &&
            val <= max,
            "prandom_double_range.") ||
    diag("Expected %f to be between %f and %f.", val, min, max);
}


void test_run() {
    test_random_int();
    test_random_char();
    test_random_long();
    test_random_llong();
    test_prandom_seed();
    test_prandom_int_range();
    test_prandom_double_range();
}
