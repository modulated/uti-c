
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"

// constructor - length of bits in array
struct bitarray bitarray_create(int size) {
	static struct bitarray ba;
	int length = ceil((float)size / (INTLENGTH * 8));
	ba.length = length;
	ba.array = (int*) calloc(length, INTLENGTH);
	return ba;
}


// deconstructor
int bitarray_destroy(struct bitarray* ba) {
	free(ba->array);
	return 0;
}

// resize array to new length;
int bitarray_resize(struct bitarray* ba, int size) {
	int length = ceil((float)size / (INTLENGTH * 8));
	ba->length = length;
	ba->array = (int*) realloc(ba->array, length * INTLENGTH);
	return 0;
}

// read position and return int value
int bitarray_read(struct bitarray* ba, int position) {

	if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
		printf("Error: bitarray out of bounds.");
		return -1;
	}

	int offset = position%(INTLENGTH*8);
	int index = (position-offset)/(INTLENGTH*8);
	int element = (ba->array[index])>>offset;
	int output = element & 0x1;
	
	return output;
}

// set to 1
int bitarray_on(struct bitarray* ba, int position) {

	if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
		printf("Error: bitarray out of bounds.");
		return -1;
	}

	int offset = position%(INTLENGTH*8);
	int index = (position-offset)/(INTLENGTH*8);
	int element = ba->array[index];
	int mask = 0x1 << offset;
	int output = element | mask;
	ba->array[index] = output;
	
	return 0;
}


// set position to 0
int bitarray_off(struct bitarray* ba, int position) {	

	if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
		printf("Error: bitarray out of bounds.");
		return -1;
	}

	int offset = position%(INTLENGTH*8);
	int index = (position-offset)/(INTLENGTH*8);
	int element = ba->array[index];
	int mask = 0x1 << offset;
	int output = element & ~mask;
	ba->array[index] = output;
	return 0;
}

// flip bit at position
int bitarray_flip(struct bitarray* ba, int position) {

	if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
		printf("Error: bitarray out of bounds.");
		return -1;
	}

	int offset = position%(INTLENGTH*8);
	int index = (position-offset)/(INTLENGTH*8);
	int element = ba->array[index];
	int mask = 0x1 << offset;
	int output = element ^ mask;
	ba->array[index] = output;
	return 0;
}



