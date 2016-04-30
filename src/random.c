/* 				TODO 
** - Include more output types (8bit, 16bit, 32bit, 64bit, unsigned, char).
** - Optimise read loop - only read urandom once.
** - Add support for argp.
*/

#include <stdio.h>
#include <stdlib.h>

int readrandomint() {

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

void printrandomint() {
	int rint = 0;
	rint = readrandomint();
	printf("%d\n",rint);
}

int main(int argc, char* argv[]) {

	if (argc==2) {
		int in = atoi(argv[1]);
		for (int i = 0; i < in; i++) {
			printrandomint();
		}
	}
	else {
		printrandomint();
	}
	
	return 0;
}
