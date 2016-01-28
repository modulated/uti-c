#include <stdio.h>
#include <stdlib.h>
#include "sieve.h"

#define MAXINT 128

unsigned int bitfield[MAXINT/sizeof(unsigned int)/8] = {0};



int main(int argc, char* argv[]) {
/*
	for (unsigned i = 2; i<MAXINT; i++) {

	}
*/
	bitflip(2,bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);

	bitflip(4, bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);

	bitflip(2, bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);	

	bitflip(4, bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);
}

/* bitmasking logic 
	
	int | 1<<(loc) = set bit at loc to 1
	0000 0000 | 0000 0010 = 0000 0010
	
	int & 1<<(loc) = set bit at loc to 0
	0000 1010 & 0000 0010 = 0000 1000

*/

void bitflip(int in, unsigned out[]) {
	// can potentially optimise modulo to rounding
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] ^ 0x1<<(mod-1);
}

void biton(int in, unsigned out[]) {
	// can potentially optimise modulo to rounding
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] | 0x1<<(mod-1);
}

void bitoff(int in, unsigned out[]) {
	// can potentially optimise modulo to rounding
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] & 0x1<<(mod-1);
}