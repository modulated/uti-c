#include "../src/stats.h"
#include <tap.h>
#include <stdlib.h>
#include <math.h>

#define TEST_SET 2, 4, 3, 2

void test_all(void);

int main() {
	plan_no_plan();
	test_all();
	
	return 0;
}

void test_set_construct() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	ok1(test_set.length == 4);
	ok1(test_set.array[3] == 2);
	
	stats_numbers_destruct(&test_set);
}

void test_set_destruct() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	stats_numbers_destruct(&test_set);

	ok1(test_set.array == NULL);	
}

void test_mean() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float x = stats_numbers_mean(test_set);
	x = roundf(x*100)/100;
	
	ok(x == 2.75, "%.2f== %.2f", x, 2.75);
	
	stats_numbers_destruct(&test_set);	
}

void test_numbers_variance() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float x = stats_numbers_variance(test_set, 2.75);
	x = roundf(x*10000)/10000;
	
	ok(x == 0.6875f, "%.4f == %.4f", x, 0.6875f);
	
	stats_numbers_destruct(&test_set);	
}

void test_numbers_sd() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float x = stats_numbers_sd(test_set, 2.75);
	x = roundf(x*10000)/10000;
	 
	ok(x == 0.8292f, "%.4f == %.4f", x, 0.8292f);
	
	stats_numbers_destruct(&test_set);	
}

void test_all(void) {
	
	test_set_construct();
	test_set_destruct();
	test_mean();
	test_numbers_variance();
	test_numbers_sd();
}
