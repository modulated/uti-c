#include "stats.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

stats_numbers stats_numbers_construct(int length, ...) {
	
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

void stats_numbers_destruct(stats_numbers* nums) {
	free(nums->array);
	nums->array = NULL;
}

float stats_numbers_mean(stats_numbers nums) {
	int total = 0;
	int i;
	
	for (i = 0; i < nums.length; i++) {
		total = total + nums.array[i];
	}
	
	return (float)total/(float)nums.length;	
}

float stats_numbers_variance(stats_numbers nums, float mean) {
		
	float total = 0.0;
	
	for (int i = 0; i < nums.length; i++) {
		// (val - mean)^2
		float deviations = (nums.array[i] - mean);
		deviations *= deviations;
		total = total + deviations;		
	}
	
	return total/(float)nums.length;
}

float stats_numbers_sd(stats_numbers nums, float mean) {

	return sqrt(stats_numbers_variance(nums, mean));
}
