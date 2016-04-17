#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void flagmult(unsigned long in, unsigned long ba[]) {
	unsigned long i = in;
	while( (i = i + in) <= MAXINT) {
		
		ba[i] = 1;
	}
}

int main(int argc, char* argv[]) {
	
    puts("Enter max number: ");
    scanf("%lu",&MAXINT);
    MAXINTSQR = (unsigned long)sqrt(MAXINT);

	// inititalise array of size MAXINT+1.
    unsigned long* ba = calloc(MAXINT+1, sizeof(unsigned long));
	
	// Exclude evens, 0 and 1.
	flagmult(2, ba);
	ba[1] = 1;
	ba[0] = 1;
	
	unsigned long counter = 1;
	
	for (unsigned long i = 3; i <= MAXINT; i = i + 2) {
		
		if (i <= MAXINTSQR) {
			flagmult(i, ba);
		}
		
		if (ba[i] == 0) {
			//printf("Prime: %lu\n", i);
			counter++;
		}
		
	}	
	
	free(ba);
	
	printfcomma(counter); 
	printf(" primes under ");
	printfcomma(MAXINT);
	puts(".");
}



