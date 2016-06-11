/* bioseq_string.c
** contains functions that manipulate strings or char arrays
*/
#include "bioseq.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Reverses given string. Modifies string (FIX).
static void bioseq_string_reverse(char* str) {
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


// Removes char at index in string. Modifies string (FIX).  
static void bioseq_string_excise(char* str, int index) {
	
	while (str[index++]) {
		str[index-1] = str[index];
	} 
}


// Adds char at index in string. Modifies string (FIX).  
static char* bioseq_string_insert(char* str, int index, char ins) {
	char* new = malloc(strlen(str)+2 * sizeof(char));

	strncpy(new, str, index);
	new[index] = ins;
	strcpy(&new[index+1], &str[index]);

	return new;
}


// Returns new string with added string at index
static char* bioseq_string_insert_chunk(char* str, int index, char* ins) {
    char* new = malloc((strlen(str) + strlen(ins) + 1) * sizeof(char));

    strncpy(new, str, index);
    strcpy(&new[index], ins);
    strcpy(&new[index + strlen(ins)], &str[index]);

    return new;
}


// Splits string at index and copies to two pointers.
static void bioseq_string_split(char* input, int index, char* output1, char* output2) {
	strncpy(output1, input, index);
	strcpy(output2, &input[index]);	
}


static void bioseq_string_capitalize (char* str) {
	int i = 0;
	while (str[i]) {
		str[i] = toupper(str[i]);
		i++;
	}
}


// Searches string for phrase and returns location in string, or -1 if not found 
static int bioseq_string_search (char* str, char cmp[]) {
	char* out = strstr(str, cmp);
	int loc = (int) (out - str);
	
	if (out) return loc;
	
	else return -1;
}

static void bioseq_string_dna_complement(char* str) {
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


int bioseq_string_similarity(char* a, char* b) {
	
	int sim = 0;
	int length_a = strlen(a);
	int length_b = strlen(b);
	
	for (int i = 0; i < length_a; i++) {
		
		if (a[i] == b[i]) continue;
		else sim++;
		
		for (int j = 0; j < length_b; j++) {
			
		}
	}
	
	return sim;
}
