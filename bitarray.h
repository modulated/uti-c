
#ifndef _BITARRAY_H
#define _BITARRAY_H

#define INTLENGTH (int)sizeof(int)

struct bitarray {
    int length; 
    int* array;
};

struct bitarray bitarray_create(int size);

void bitarray_destroy(struct bitarray* ba);

int bitarray_read(struct bitarray* ba, int position);

void bitarray_on(struct bitarray* ba, int position);

void bitarray_off(struct bitarray* ba, int position);

void bitarray_flip(struct bitarray* ba, int position);

#endif

