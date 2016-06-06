#include "stats.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "random.h"

#define ARRAY_TO_ARGS(x, y) for(int i = 0; i < x; i++) {y[x]}

stats_numbers stats_numbers_construct_varargs(int length, ...) {
	
	va_list ap;
	va_start(ap, length);
	
	stats_numbers nums;
	nums.length = length;	
	nums.array = malloc(length * sizeof(int));
	
	for (int i = 0; i < length; i++) {
		nums.array[i] = va_arg(ap, int);		
	}
	
	va_end(ap);
	
	return nums;
}

stats_numbers stats_numbers_construct_array(int length, int array[]) {

	
	stats_numbers nums;
	nums.length = length;	
	nums.array = malloc(length * sizeof(int));
	
	for (int i = 0; i < length; i++) {
		nums.array[i] = array[i];		
	}
	
	return nums;
}

void stats_numbers_destruct(stats_numbers* nums) {
	free(nums->array);
	nums->array = NULL;
}

void stats_numbers_print(stats_numbers nums) {
	for (int i = 0; i < nums.length-1; i++) {
		printf("%d, ", nums.array[i]);
	}
	printf("%d.\n", nums.array[nums.length-1]);
}

void stats_int_swap(int* a, int* b) {
	
	int x = *a;
	*a = *b;
	*b = x;
}

stats_numbers stats_numbers_sort(stats_numbers nums) {
	
	
	for (int i = 0; i < nums.length-1; i++) {		
		
		if (i < 0) i = 0;
		
		if (nums.array[i] > nums.array[i+1]) {
			stats_int_swap(&nums.array[i], &nums.array[i+1]);
			i -= 2;
		}
	}	
	
	return nums;
}

float stats_numbers_mean(stats_numbers nums) {
	int total = 0;
	int i;
	
	for (i = 0; i < nums.length; i++) {
		total = total + nums.array[i];
	}
	
	return (float)total/(float)nums.length;	
}

float stats_numbers_variance(stats_numbers nums) {
	
	float mean = stats_numbers_mean(nums);
	float total = 0.0;
	
	for (int i = 0; i < nums.length; i++) {
		// (val - mean)^2
		float deviations = (nums.array[i] - mean);
		deviations *= deviations;
		total = total + deviations;		
	}
	
	return total/(float)nums.length;
}

float stats_numbers_sd(stats_numbers nums) {
		
	return sqrt(stats_numbers_variance(nums));
}


stats_numbers stats_numbers_random(int length, int min, int max) {
	
	stats_numbers out;
	int array[length];
	
	for (int i = 0; i < length; i++) {
		
		int val = random_int();
		
		long int_range = (long)INT_MAX - (long)INT_MIN;
		long new_range = max - min;
		
		long intermediate = ((long)val - (long)INT_MIN) * new_range;		
		long divisor = (intermediate / int_range) + min;
				
		array[i] = (int)divisor;				
	}
	
	out = stats_numbers_construct_array(length, array);
	stats_numbers_print(out);
	printf("mean: %f\n", stats_numbers_mean(out));
	return out;
}
