#include <stdio.h>
#include "bitarray.h"

int main() {

    int length;
    printf("How many bits? ");
    scanf("%d",&length);

    struct bitarray ba;
    struct bitarray* pointer = &ba;
    ba = bitarray_create(length);
    
    printf("Bitarray size: %d bytes\n", ba.length * INTLENGTH);
    printf("Array length: %d bits\n", ba.length * 8 * INTLENGTH);

    bitarray_flip(pointer, 0);
    printf("Position 0: %d\n",bitarray_read(pointer, 0));
    printf("array[0]: %d\n", ba.array[0]);
    bitarray_flip(pointer, length);

    bitarray_destroy(pointer);
    return 0;
}
