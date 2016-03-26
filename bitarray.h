
#ifndef _BITARRAY_H
#define _BITARRAY_H

#define INTLENGTH (int)sizeof(int)

struct bitarray {
    int length; 
    int* array;
};

struct bitarray createbitarray(int size);

void destroybitarray(struct bitarray* ba);

void bitarraytoggle(struct bitarray* ba, int position);

void bitarrayon(struct bitarray* ba, int position);

void bitarrayoff(struct bitarray* ba, int position);

int bitarrayread(struct bitarray* ba, int position);

#endif

