
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
void bitarray_destroy(struct bitarray* ba) {
    free(ba->array);
}

// read position and return int value
int bitarray_read(struct bitarray* ba, int position) {
    
    int length = ba->length;

    if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
        printf("Error: bitarray out of bounds.");
        exit(1);
    }

    int offset = position%(INTLENGTH*8);
    int index = (position-offset)/(INTLENGTH*8);
    int element = (ba->array[index])>>offset;
    int output = element & 0x1;
    
    return output;
}

// set to 1
void bitarray_on(struct bitarray* ba, int position) {
    
    int length = ba->length;

    if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
        printf("Error: bitarray out of bounds.");
        exit(1);
    }

    int offset = position%(INTLENGTH*8);
    int index = (position-offset)/(INTLENGTH*8);
    int element = ba->array[index];
    int mask = 0x1 << offset;
    int output = element | mask;
    ba->array[index] = output;
}


// set position to 0
void bitarray_off(struct bitarray* ba, int position) {
    
    int length = ba->length;

    if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
        printf("Error: bitarray out of bounds.");
        exit(1);
    }

    int offset = position%(INTLENGTH*8);
    int index = (position-offset)/(INTLENGTH*8);
    int element = ba->array[index];
    int mask = 0x1 << offset;
    int output = element & ~mask;
    ba->array[index] = output;
}

// flip position
void bitarray_flip(struct bitarray* ba, int position) {

    int length = ba->length;
    
    if (position >= (INTLENGTH * ba->length * 8) || position < 0) {
        printf("Error: bitarray out of bounds.");
        exit(1);
    }

    int offset = position%(INTLENGTH*8);
    int index = (position-offset)/(INTLENGTH*8);
    int element = ba->array[index];
    int mask = 0x1 << offset;
    int output = element ^ mask;
    ba->array[index] = output;
}



