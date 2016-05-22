#ifndef STATS_H
#define STATS_H

typedef struct {
	int* array;
	int length;
} stats_numbers;

stats_numbers stats_numbers_construct(int length, ...);

void stats_numbers_destruct(stats_numbers* nums);

float stats_numbers_mean(stats_numbers nums);

float stats_numbers_variance(stats_numbers nums, float mean);

float stats_numbers_sd(stats_numbers nums, float mean);


#endif
