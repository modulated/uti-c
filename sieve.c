#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"

#define MAXINT (unsigned long)4000
#define MAXINTSQR (unsigned long)sqrt(MAXINT)

unsigned long bitfield[MAXINT/sizeof(unsigned long)/8] = {0};

void flagmult(unsigned long in, unsigned long array[]) {
	unsigned long i = in;
	while( (i = i + in) <= MAXINT) {
		biton(i, array);
	}
}

int main(int argc, char* argv[]) {
	
	unsigned long i;
	
	for (i = 2; i <= MAXINTSQR; i++) {
		printf("Multing %lu\n",i);
		flagmult(i, bitfield);
	}
	
	unsigned long count = 0;
	
	for (i = 2; i <= MAXINT; i++) {
		unsigned long out = bitread(i, bitfield);
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


