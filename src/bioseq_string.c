/* bioseq_string.c
** contains functions that manipulate strings or char arrays
*/
#include "bioseq.h"
#include <string.h>
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