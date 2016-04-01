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
	    ba[i] = 1;
	}
}

int main(int argc, char* argv[]) {
    
    printf("Enter max number: ");
    scanf("%lu",&MAXINT);
    MAXINTSQR = (unsigned long)sqrt(MAXINT);

    unsigned long ba[MAXINT+1];
	unsigned long i;
	
	for (i = 2; i <= MAXINTSQR; i++) {
		printf("Multing %lu\n",i);
		flagmult(i, ba);
	}
	
	unsigned long count = 0;
	
	for (i = 2; i <= MAXINT; i++) {
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


