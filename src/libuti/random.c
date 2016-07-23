#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void* random_bytes(int length) {
		
	void* out = malloc(length);
	 
	FILE* file;
	file = fopen("/dev/urandom","r");
	
	if(file==NULL) {
		printf("Error opening file. Program terminating\n");
		exit(1);
	}

	fread(out, length, 1, file);
	fclose(file);

	return out;
}

int random_int() {
	void* ptr = random_bytes(sizeof(int));
	int out = *(int*)ptr;
	free(ptr);	
	return out;
}

char random_char() {
	void* ptr = random_bytes(sizeof(char));
	char out = *(char*)ptr;
	free(ptr);	
	return out;
}

char random_long() {
	void* ptr = random_bytes(sizeof(long));
	long out = *(long*)ptr;
	free(ptr);	
	return out;
}

char random_llong() {
	void* ptr = random_bytes(sizeof(long long));
	long long out = *(long long*)ptr;
	free(ptr);	
	return out;
}

/*
**  Pseudorandom
*/

void prandom_seed(unsigned int seed)
{
    srandom(seed);
}

int prandom_int()
{
    return (int) random();
}

int prandom_int_range(int min, int max)
{
    int input = prandom_int();
    double slope = (max - min) / (RAND_MAX - 0);
    
    int output = floor(slope * input + 0.5);
    output += min;

    return output;
}

double prandom_double()
{
    double out = random();
    return out;
}

double prandom_double_range(double min, double max)
{
    double input = prandom_double();
    double slope = (max - min) / (RAND_MAX - 0);

    double output = slope * input + min;
    return output;
}

