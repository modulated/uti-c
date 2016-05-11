#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bioseq.h"

#define BUFFER_SIZE 1024

void bioseq_reverse_string(char* str);
void bioseq_complement_string(char* str);
void bioseq_capital_string(char* str);

bioseq bioseq_new(sequence_type type, char seq[]) {
	
	int len = strlen(seq);
	char* string = malloc((len) * sizeof(char));
	strcpy(string, seq);
	
	bioseq_capital_string(string);
	
	bioseq new = {
		type,
		len,
		string
	};
	
	return new;
}

void bioseq_delete(bioseq seq) {
	free(seq.sequence);
}

bioseq bioseq_reverse(bioseq seq) {
	
	int len = seq.length;
	char string[len];
	strcpy(string, seq.sequence);
	
	bioseq_reverse_string(string);
	
	return bioseq_new(seq.type, string);
}

bioseq bioseq_complement(bioseq seq) {
	
	int len = seq.length;
	char string[len];
	strcpy(string, seq.sequence);
	
	bioseq_complement_string(string);
	
	return bioseq_new(seq.type, string);
	
}

int main() {
	char buffer[BUFFER_SIZE];
	
	
	puts("Bioseq Tester");
	printf("Seq: ");
	scanf("%s", buffer);
	
	
	bioseq test = bioseq_new(SEQUENCE_NONE, buffer);
	
	printf("Sequence: %s \tLen: %u\n", test.sequence, test.length);
	printf("Reversed: %s\n", bioseq_reverse(test).sequence);
	printf("Complement: %s\n", bioseq_complement(test).sequence);
	
	bioseq_delete(test);
	
	
	
	return 0;
}

void bioseq_reverse_string(char* str)
{
    /* skip null */
    if (str == 0)
    {
    	return;
    }

    /* skip empty string */
    if (*str == 0)
    {
    	return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
    	/* swap */
    	temp = *start;
    	*start = *end;
    	*end = temp;

    	/* move */
    	++start;
    	--end;
    }
}

void bioseq_complement_string(char* str) {
	int i = 0;
	while (str[i]) {
		switch(str[i]) {
			case 'A' : 
				str[i] = 'T'; 
				break;
			case 'C' : 
				str[i] = 'G';
				break;
			case 'G' : 
				str[i] = 'C';
				break;
			case 'T' : 
				str[i] = 'A';
				break;
		}
		
		i++;
	}
	
}

void bioseq_capital_string (char* str) {
	int i = 0;
	while (str[i]) {
		str[i] = toupper(str[i]);
		i++;
	}
}