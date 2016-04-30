
#ifndef _BITARRAY_H
#define _BITARRAY_H

#define INTLENGTH (int)sizeof(int)

struct bitarray {
    int length; 
    int* array;
};

struct bitarray bitarray_create(int size);

int bitarray_destroy(struct bitarray* ba);

int bitarray_read(struct bitarray* ba, int position);

int bitarray_on(struct bitarray* ba, int position);

int bitarray_off(struct bitarray* ba, int position);

int bitarray_flip(struct bitarray* ba, int position);

int bitarray_resize(struct bitarray* ba, int size);

#endif

