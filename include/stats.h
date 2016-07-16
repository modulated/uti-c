#ifndef _STATS_H
#define _STATS_H

typedef struct {
	int* array;
	int length;
} stats_numbers;

stats_numbers stats_numbers_construct_varargs(int length, ...);

stats_numbers stats_numbers_construct_array(int length, int array[]);

void stats_numbers_destruct(stats_numbers* nums);

stats_numbers stats_numbers_sort(stats_numbers nums);

float stats_numbers_mean(stats_numbers nums);

float stats_numbers_variance(stats_numbers nums);

float stats_numbers_sd(stats_numbers nums);

void stats_numbers_print(stats_numbers nums);

stats_numbers stats_numbers_random(int length, int min, int max);


#endif
