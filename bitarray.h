#ifndef _BITARRAY_H
#define _BITARRAY_H

#define INTLENGTH (int)sizeof(int)

struct bitarray {int length, int* array};

struct bitarray createbitarray(int size);

void destroybitarray(struct bitarray ba);

void bitarrayon(struct bitarray* ba, position);

#endif
