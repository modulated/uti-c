#include "tap.h"
#include "../src/stats.h"
#include <stdlib.h>
#include <math.h>

#define TEST_SET 2, 4, 3, 2

void test_all(void);

int main() {
	plan_no_plan();
	test_all();
	
	return exit_status();
}

void test_numbers_construct() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	diag("stats_numbers_construct");
	ok1(test_set.length == 4);
	ok1(test_set.array[3] == 2);
	
	stats_numbers_destruct(&test_set);
}

void test_numbers_destruct() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	stats_numbers_destruct(&test_set);

	ok(test_set.array == NULL, "stats_set_destruct: sets array pointer NULL");	
}

void test_numbers_sort() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	test_set = stats_numbers_sort(test_set);
	
	int expected_3 = test_set.array[2];	
	int expected_4 = test_set.array[3];
	
	ok(expected_4 == 4 && expected_3 == 3, "stats_numbers_sort: Returns expected value") || diag("Expecting: %d", 4) || diag("Actual: %d", expected_4);
	
	stats_numbers_destruct(&test_set);		
}

void test_numbers_mean() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float x = stats_numbers_mean(test_set);
	x = roundf(x*100)/100;
	
	ok(x == 2.75f, "stats_numbers_mean: Returns expected value") || diag("Expecting: %f", 2.75f) || diag("Actual: %f", x);
	
	stats_numbers_destruct(&test_set);	
}

void test_numbers_variance() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float expected = 0.6875f;
	
	float actual = stats_numbers_variance(test_set, 2.75);
	actual = roundf(actual*10000)/10000;
	
	ok(actual == expected, "stats_numbers_variance: Returns expected value") || diag("Expecting: %f", expected) || diag("Actual:%f", actual);
	
	stats_numbers_destruct(&test_set);
}

void test_numbers_sd() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float x = stats_numbers_sd(test_set, 2.75);
	x = roundf(x*10000)/10000;
	 
	ok(x == 0.8292f, "stats_numbers_sd: Returns expected value") || diag("Expecting: %f", 0.8292f) || diag("Actual:%f", x);	
	
	stats_numbers_destruct(&test_set);	
}

void test_all(void) {
	
	test_numbers_construct();
	test_numbers_destruct();
	test_numbers_sort();
	test_numbers_mean();
	test_numbers_variance();
	test_numbers_sd();
}
