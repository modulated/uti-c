#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"

#define MAXINT (unsigned long)4000000
#define MAXINTSQR (unsigned long)sqrt(MAXINT)

void flagmult(unsigned long in, struct bitarray* ba) {
	unsigned long i = in;
	while( (i = i + in) <= MAXINT) {
		bitarray_on(ba, (int)i);
	}
}

int main(int argc, char* argv[]) {
	
    struct bitarray ba = bitarray_create(MAXINT+1);
	unsigned long i;
	
	for (i = 2; i <= MAXINTSQR; i++) {
		printf("Multing %lu\n",i);
		flagmult(i, &ba);
	}
	
	unsigned long count = 0;
	
	for (i = 2; i <= MAXINT; i++) {
		unsigned long out = (unsigned long)bitarray_read(&ba,(int)i);
		if (out == 0) {
			printf("Prime: %lu\n", i);
			count++;
		}
	}
	
	
	printf("Count: %lu\n",count);
}

/* Pseudo

init bitarray[MAXINT]
iterate through bitarary elements up to sqrt(MAXINT)
flag multiples of iteration but not iteration itself
count


*/


