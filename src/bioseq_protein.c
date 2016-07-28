/* bioseq_protein.c
** contains functions acting on protein sequences
*/

#include "bioseq.h"
#include <stdlib.h>
#include <string.h>

bioseq_protein bioseq_protein_construct(char seq[]) {
	
	if (seq != NULL) {
		int len = strlen(seq);
		char* charge = malloc(len * sizeof(char));
		
		bioseq_protein new = {
			len,
			strdup(seq),
			charge
		};
		
		bioseq_string_capitalize(new.sequence);	
		bioseq_protein_sanitize(new);
		new.length = strlen(seq);
		
		return new;
	}

	else {
		return bioseq_protein_construct("");
	}
}



void bioseq_protein_destruct(bioseq_protein* seq) {
	// Free allocated heap and set pointers to NULL	
	
	free(seq->sequence);
	seq->sequence = NULL;
	
	free(seq->charge);
	seq->charge = NULL;
}



int bioseq_protein_startcodon(bioseq_protein seq) {
	return bioseq_string_search(seq.sequence, "M");
}



void bioseq_protein_split(bioseq_protein seq, unsigned int index, bioseq_protein* out1, bioseq_protein* out2) {
	
	if (index > seq.length) index = seq.length;
		
	char* str1 = calloc((index + 1), sizeof(char));
	char* str2 = calloc((seq.length - index + 1), sizeof(char)); 
	bioseq_string_split(seq.sequence, index, str1, str2);    

	*out1 = bioseq_protein_construct(str1);
	*out2 = bioseq_protein_construct(str2);
	
	free(str1);
	free(str2);	
}



bioseq_protein bioseq_protein_join (bioseq_protein start, bioseq_protein end) {
	
	if (end.sequence == NULL && start.sequence == NULL) return bioseq_protein_construct("");
	if (end.sequence == NULL) return bioseq_protein_construct(start.sequence);
	if (start.sequence == NULL) return bioseq_protein_construct(end.sequence);

	bioseq_protein out;
	char* string_cat = malloc((start.length + end.length + 1) * sizeof(char));
	strcpy(string_cat, start.sequence);
	strcat(string_cat, end.sequence);
	out = bioseq_protein_construct(string_cat);
	
	free(string_cat);
	return out;
}



void bioseq_protein_interactions(bioseq_protein seq) {
	
	for (unsigned int i = 0; i < seq.length; i++) {
		seq.charge[i] = bioseq_protein_charge(seq.sequence[i]);
	}
}



void bioseq_protein_sanitize(bioseq_protein seq) {
	
	char* str = seq.sequence;
	int i = 0;
	
	while (str[i] != '\0') {
		
		if (bioseq_protein_verifychar(str[i]) == 0) {
			
			bioseq_string_excise(str, i);			
		}
		
		else {			
			i++;
		}
	}
}


/* Returns protein charge for given single letter code.
+ = positive, - = negative, # = hydrophobic, * = hydrophilic */
char bioseq_protein_charge (char in) {
	switch (in) {
		
		// Positive electric charge
		case 'R': 
		case 'H':
		case 'K':
			return '+';
		
		// Positive electric charge
		case 'D':
		case 'E':
			return '-';
		
		// Hydrophobic	
		case 'A':
		case 'C':
		case 'F':
		case 'G':
		case 'I':
		case 'L':
		case 'M':
		case 'P':
		case 'V':
		case 'W':
		case 'Y':
			return '#';
			
		// Hydrophilic
		case 'N':
		case 'S':
		case 'T':
		case 'Q':
			return '*';
		
		default: 
			return '?';
	}
}


// Returns 1 if char matches single letter protein code or 0 if not. 
int bioseq_protein_verifychar(char x) {
	
	switch(x) {
		case 'A': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'L': case 'K': case 'M': case 'N': case 'Q': case 'P': case 'R': case 'S': case 'T': case 'V': case 'W': case 'Y': case 'X':
			return 1;			
	}
	return 0;
}



void bioseq_protein_terminate (char* str) {
	int i = 0;
	while (str[i++]) {
		if (str[i] == 'X') {
			str[i] = '\0';
			break;
		}
	}
	
}
