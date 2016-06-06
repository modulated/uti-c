#include "tap.h"
#include "../src/stats.h"
#include <stdlib.h>
#include <math.h>

#define TEST_SET 2, 4, 3, 1

void test_all(void);

int main() {
	plan_no_plan();
	test_all();
	
	return exit_status();
}

void test_numbers_construct() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);	
	ok1(test_set.length == 4);
	ok1(test_set.array[3] == 1);
	
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
	stats_numbers_print(test_set);
	
	ok(expected_4 == 4 && expected_3 == 3, "stats_numbers_sort: Returns expected value") || diag("Expecting: %d and %d", 3, 4) || diag("Actual: %d and %d", expected_3, expected_4);
	
	stats_numbers_destruct(&test_set);		
}

void test_numbers_mean() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float expected = 2.5f;
	
	float actual = stats_numbers_mean(test_set);
	actual = roundf(actual*100)/100;
	
	ok(actual == expected, "stats_numbers_mean: Returns expected value") || diag("Expecting: %f", expected) || diag("Actual: %f", actual);
	
	stats_numbers_destruct(&test_set);	
}

void test_numbers_variance() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float expected = 1.25f;
	
	float actual = stats_numbers_variance(test_set);
	actual = roundf(actual*10000)/10000;
	
	ok(actual == expected, "stats_numbers_variance: Returns expected value") || diag("Expecting: %f", expected) || diag("Actual:%f", actual);
	
	stats_numbers_destruct(&test_set);
}

void test_numbers_sd() {
	
	stats_numbers test_set = stats_numbers_construct(4, TEST_SET);
	float expected = 1.1180;
	
	float actual = stats_numbers_sd(test_set);
	actual = roundf(actual*10000)/10000;
	 
	ok(actual == expected, "stats_numbers_sd: Returns expected value") || diag("Expecting: %f", expected) || diag("Actual:%f", actual);	
	
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
