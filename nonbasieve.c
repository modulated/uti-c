#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"

//#define MAXINT (unsigned long)4000000
//#define MAXINTSQR (unsigned long)sqrt(MAXINT)

unsigned long MAXINT;
unsigned long MAXINTSQR;

void flagmult(unsigned long in, unsigned long ba[]) {
	unsigned long i = in;
	while( (i = i + in) <= MAXINT) {
		
		if (ba[i] == 0) {
	    	ba[i] = 1;
			printf("Flip at %lu, multing %lu\n", i, in);			
		}
	}
}

int main(int argc, char* argv[]) {
    
    printf("Enter max number: ");
    scanf("%lu",&MAXINT);
    MAXINTSQR = (unsigned long)sqrt(MAXINT);

    unsigned long* ba = calloc(MAXINT+1, sizeof(unsigned long));
	
	// Exclude evens, 0 and 1.
	flagmult(2, ba);
	ba[1] = 1;
	ba[0] = 1;
	
	for (unsigned long i = 3; i <= MAXINTSQR; i = i + 2) {
		printf("Multing %lu\n",i);
		flagmult(i, ba);
	}
	
	unsigned long count = 1;
	printf("Prime: 2\n");
	
	for (unsigned long i = 3; i <= MAXINT; i = i + 2) {
		unsigned long out = ba[i];
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


