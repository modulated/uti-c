#ifndef _RANDOM_H
#define _RANDOM_H

// Returns pointer to arbitrary number of bytes. Need to free().
void* random_bytes(int length);

// Returns random integer.
int random_int();

int random_int_range(int min, int max);

// Returns random char.
char random_char();

// Returns random long int.
long random_long();

// Returns random long long int.
long long random_llong();

// Returns random float.
float random_float();

// Returns random double float.
double random_double();

double random_double_range();

/*
** Psuedo Random - seeded
*/

void prandom_seed(unsigned int seed);

int prandom_int();

int prandom_int_range(int min, int max);

double prandom_double();

double prandom_double_range(double min, double max);

#endif
