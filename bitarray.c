
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"


// constructor - length of bits in array
struct bitarray createbitarray(int size) {
    static struct bitarray ba;
    int length = ceil((float)size / (INTLENGTH * 8));
    ba->length = length;
    ba->array = (int*) calloc(length, INTLENGTH);
    return ba;
}

//
// deconstructor
void destroybitarray(struct bitarray* ba) {
    free(ba->array);
}
// set to 0
//
// set to 1
void bitarrayon(struct bitarray* ba, position) {
    
    int length = ba->length;

    if (position >= (INTLENGTH * ba->length * 8)) {
        printf("Error: bitarray out of bounds.");
        exit(1);
    }

    int offset = position%(INTLENGTH*8);
    int index = (position-offset)/(INTLENGTH*8);
    int element = ba->array[index];
    int mask = 0x1 << offset;
    int output = element && mask;
    ba->array[index] = output;
}
// bitflip

//init to oall 0


