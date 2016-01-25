#include <stdio.h>
#include <stdlib.h>
#define MAXINT 128

unsigned int bitfield[MAXINT/sizeof(unsigned int)/8] = {0};
void biton(int in, unsigned out[]);


int main(int argc, char* argv[]) {
/*
	for (unsigned i = 2; i<MAXINT; i++) {

	}
*/
	biton(1,bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);

	biton(3, bitfield);
	printf("Bitfield[0]: %d\n",bitfield[0]);

}

void biton(int in, unsigned out[]) {
	// can potentially optimise modulo to round
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = 1<<(mod-1);
}
