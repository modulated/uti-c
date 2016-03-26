#include <stdio.h>
#include "bitarray.h"

int main() {
    int length;
    printf("How many bits? ");
    scanf("%d \n",&length);
    bitarray ba;
    ba = createbitarray(length);
    printf("Bitarray[0]: %d\n", ba[2]);
    printf("Int length: %d\n", sizeof(int));
    printf("Array length: %d\n", (int)sizeof(ba) / INTLENGTH);
    destroybitarray(ba);
    return 0;
}
