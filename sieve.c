#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bitarray.h"

unsigned long MAXINT;
unsigned long MAXINTSQR;

void printfcomma (int n) {
	if (n < 0) {
		printf ("-");
		printfcomma (-n);
		return;
	}
	if (n < 1000) {
		printf ("%d", n);
		return;
	}
	printfcomma (n/1000);
	printf (",%03d", n%1000);
}

void flagmult(unsigned long in, struct bitarray* ba) {
	unsigned long i = in;
	while( (i = i + in) <= MAXINT) {
		
		bitarray_on(ba, i);
	}
}

int main(int argc, char* argv[]) {
	
	if (argc == 1) {
		puts("Enter max number: ");
		scanf("%lu",&MAXINT);
	}
	else {
		if( (MAXINT = strtoul(argv[1],NULL,10)) == 0) {
			puts("Error parsing input");
			exit(1);
		}
	}
		
	MAXINTSQR = (unsigned long)sqrt(MAXINT);

	if (MAXINT > (int)MAXINT) {printf("Overflow!");}
	
	// inititalise array of size MAXINT+1.
	struct bitarray ba = bitarray_create((int)MAXINT+1);
	
	// start timing
	clock_t t1, t2;
	t1 = clock();
	
	// Exclude evens, 0 and 1.
	flagmult(2, &ba);
	bitarray_on(&ba, 0);
	bitarray_on(&ba, 1);
	
	unsigned long counter = 1;
	
	for (unsigned long i = 3; i <= MAXINT; i = i + 2) {
		
		if (i <= MAXINTSQR) {
			flagmult(i, &ba);
		}
		
		if (bitarray_read(&ba, i) == 0) {
			//printf("Prime: %lu\n", i);
			counter++;
		}
		
	}	
	
	t2 = clock();
	float diff = ((float)(t2-t1)/CLOCKS_PER_SEC)*1000;
	
	bitarray_destroy(&ba);
	
	printfcomma(counter); 
	printf(" primes under ");
	printfcomma(MAXINT);
	printf(" in %.3f milliseconds.\n", diff);
}






