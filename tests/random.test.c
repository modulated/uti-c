/*
** Need to implement tests of randomness.
*/

#include "../lib/tap.h"
#include <limits.h>
#include "../src/random.h"

void test_run(void);

int main() {
    plan_tests(4);

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

void test_run() {
    test_random_int();
    test_random_char();
    test_random_long();
    test_random_llong();
}
