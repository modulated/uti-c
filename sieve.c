#include <stdio.h>
#include <stdlib.h>
#include "bitarray.h"

#define MAXINT 128

unsigned int bitfield[MAXINT/sizeof(unsigned int)/8] = {0};



int main(int argc, char* argv[]) {

	bitflip(2,bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);

	bitflip(127, bitfield);
	//printf("Bitfield[0]: %d\n",bitfield[0]);
	
	printf("%d: %d\n",127,bitread(127, bitfield));

	bitflip(2, bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);	

	bitflip(4, bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);
	
	printf("%d: %d\n",4,bitread(4, bitfield));
}





