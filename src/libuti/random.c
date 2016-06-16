/* 				TODO 
** - Include more output types (8bit, 16bit, 32bit, 64bit, unsigned, char).
** - Optimise read loop - only read urandom once.
*/

#include <stdio.h>
#include <stdlib.h>

void* random_bytes(int length) {
		
	void* out = malloc(length);
	 
	FILE* file;
	file = fopen("/dev/urandom","r");
	
	if(file==NULL) {
		printf("Error opening file. Program terminating\n");
		exit(1);
	}

	fread(out, length, 1, file);
	fclose(file);

	return out;
}

int random_int() {
	void* ptr = random_bytes(sizeof(int));
	int out = *(int*)ptr;
	free(ptr);	
	return out;
}

char random_char() {
	void* ptr = random_bytes(sizeof(char));
	char out = *(char*)ptr;
	free(ptr);	
	return out;
}

char random_long() {
	void* ptr = random_bytes(sizeof(long));
	long out = *(long*)ptr;
	free(ptr);	
	return out;
}

char random_llong() {
	void* ptr = random_bytes(sizeof(long long));
	long long out = *(long long*)ptr;
	free(ptr);	
	return out;
}
