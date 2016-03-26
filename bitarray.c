
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"

// constructor - length of bits in array
struct bitarray createbitarray(int size) {
    static struct bitarray ba;
    int length = ceil((float)size / (INTLENGTH * 8));
    ba.length = length;
    ba.array = (int*) calloc(length, INTLENGTH);
    return ba;
}


// deconstructor
void destroybitarray(struct bitarray* ba) {
    free(ba->array);
}

// set to 1
void bitarrayon(struct bitarray* ba, int position) {
    
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

// read position and return int value
int bitarrayread(struct bitarray* ba, int position) {
    
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
// set position to 0
void bitarrayoff(struct bitarray* ba, int position) {
    
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
void bitarraytoggle(struct bitarray* ba, int position) {

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



