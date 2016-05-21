/* 				TODO 
** - Include more output types (8bit, 16bit, 32bit, 64bit, unsigned, char).
** - Optimise read loop - only read urandom once.
*/

#include <stdio.h>
#include <stdlib.h>

int random_int() {

	unsigned char rint = 0;
	FILE* file;

	file = fopen("/dev/urandom","r");
	if(file==NULL) {
		printf("Error opening file. Program terminating\n");
		exit(1);
	}

	//Read and parse

	fread(&rint, sizeof(rint), 1, file);
	fclose(file);

	return rint;
}


