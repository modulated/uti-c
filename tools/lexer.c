#include <syn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes .san file input and generates .syn code */

int lexfile(char* file)
{

	return 0;
}

int verify_extention(char* input, char* ext)
{
	if (strlen(ext) != 3) return -1;

	int i = 0;
	for (i = 0; i < strlen(input); i++)
	{
		if (input[i] == '.') break;
	}

	for (int j = 0; j < 3; j++)
	{
		if (input[i + j + 1] != ext[j]) return 0;
	}

	return 1;
}

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <file>.san\n", argv[0]);
	}
	
	FILE* infile = fopen(argv[1], "r");
	char outfile_name[strlen(argv[1])];
	strncpy(outfile_name, argv[1], strlen(argv[1] - 3));
	strncpy(&outfile_name[strlen(argv[1])-3], "syn", 3);
	
	printf("Mangled name: %s\n", outfile_name);
	
	FILE* outfile = fopen(outfile_name, "wb");


	if (lexfile(argv[1]) != 0)
	{
		printf("Error parsing %s\n", argv[1]);
	}

	if (verify_extention(argv[1], "syn"))
		

	fclose(outfile);
	fclose(infile);
	return 0;
}
