#include <tap.h>
#include <limits.h>
#include "../src/random.h"

void test_run(void);

int main() {
    plan_no_plan();

    test_run();
}

void test_random_int() {
    
    int long total;
    for (int i = 0; i < 100; i++) {

        int test_int = random_int();
        ok1(test_int >= INT_MIN && test_int <= INT_MAX);
        total += test_int;
    }
    
}

void test_run() {
    test_random_int();
}
