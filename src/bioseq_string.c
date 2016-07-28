/* bioseq_string.c
** contains functions that manipulate strings or char arrays
*/
#include "bioseq.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dmp.h>

// Reverses given string. Modifies string (FIX).
void bioseq_string_reverse(char* str) {
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
void bioseq_string_excise(char* str, int index) {
	
	while (str[index++]) {
		str[index-1] = str[index];
	} 
}


// Adds char at index in string. Modifies string (FIX).  
char* bioseq_string_insert(char* str, int index, char ins) {
	char* new = malloc(strlen(str)+2 * sizeof(char));

	strncpy(new, str, index);
	new[index] = ins;
	strcpy(&new[index+1], &str[index]);

	return new;
}


// Returns new string with added string at index
char* bioseq_string_insert_chunk(char* str, int index, char* ins) {
	char* new = malloc((strlen(str) + strlen(ins) + 1) * sizeof(char));

	strncpy(new, str, index);
	strcpy(&new[index], ins);
	strcpy(&new[index + strlen(ins)], &str[index]);

	return new;
}


// Splits string at index and copies to two pointers.
void bioseq_string_split(char* input, int index, char* output1, char* output2) {
	strncpy(output1, input, index);
	strcpy(output2, &input[index]);	
}


void bioseq_string_capitalize (char* str) {
	int i = 0;
	while (str[i]) {
		str[i] = toupper(str[i]);
		i++;
	}
}


// Searches string for phrase and returns location in string, or -1 if not found 
int bioseq_string_search (char* str, char cmp[]) {
	char* out = strstr(str, cmp);
	int loc = (int) (out - str);
	
	if (out) return loc;
	
	else return -1;
}

void bioseq_string_dna_complement(char* str) {
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

int bioseq_diff_equal(void* ref, dmp_operation_t op, const void* data, uint32_t len) {
	
	int* sum = ref;

	if (op == DMP_DIFF_EQUAL) {
		*sum += len;
	}
	
	return 0;
}

int bioseq_string_similarity(char* a, char* b) {

	dmp_diff* diff = NULL;
	int eq = 0;

	if (dmp_diff_from_strs(&diff, NULL, a, b) != 0) {
		puts("ERROR: dmp_diff");
		exit(1);
	}

	dmp_diff_foreach(diff, bioseq_diff_equal, &eq);

	dmp_diff_free(diff);
	
	return eq;
}

int bioseq_diff_insert(void* ref, dmp_operation_t op, const void* data, uint32_t len) {
	
	int* sum = ref;

	if (op == DMP_DIFF_INSERT) {
		*sum += len;
	}

	return 0;
}

int bioseq_string_insertions(char* a, char* b) {

	dmp_diff* diff = NULL;
	int eq = 0;

	if (dmp_diff_from_strs(&diff, NULL, a, b) != 0) {
		puts("ERROR: dmp_diff");
		exit(1);
	}
	
	dmp_diff_foreach(diff, bioseq_diff_insert, &eq);
	
	dmp_diff_free(diff);
	
	return eq;
}

int bioseq_diff_deletions(void* ref, dmp_operation_t op, const void* data, uint32_t len) {
	
	int* sum = ref;

	if (op == DMP_DIFF_DELETE) {
		*sum += len;
	}

	return 0;
}

int bioseq_string_deletions(char* a, char* b) {

	dmp_diff* diff = NULL;
	int eq = 0;

	if (dmp_diff_from_strs(&diff, NULL, a, b) != 0) {
		puts("ERROR: dmp_diff");
		exit(1);
	}
	
	dmp_diff_foreach(diff, bioseq_diff_deletions, &eq);
	
	dmp_diff_free(diff);
	
	return eq;
}
