#include <stdio.h>
#include "bitarray.h"

int main() {

    int length;
    printf("How many bits? ");
    scanf("%d",&length);

    struct bitarray ba;
    ba = createbitarray(length);
    
    printf("You entered: %d\n", length);
    printf("Bitarray[0]: %d\n", ba.array[0]);
    printf("Int length: %d\n", (int)sizeof(int));
    printf("Array length: %d\n", ba.length * 8 * INTLENGTH);

    bitarrayon(&ba, 2); 
    bitarraytoggle(&ba, 3);
    bitarraytoggle(&ba, 2);
    printf("Value at pos 3: %d\n", bitarrayread(&ba, 3));
    printf("Value at last position: %d\n", bitarrayread(&ba, ba.length)); 
    printf("Bitarray[0]: %d\n", ba.array[0]);

    destroybitarray(&ba);
    return 0;
}
