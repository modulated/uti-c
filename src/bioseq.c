#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bioseq.h"

#define BUFFER_SIZE 1024
#define START_CODON "ATG"

void bioseq_reverse_string(char* str);
void bioseq_complement_string(char* str);
void bioseq_capital_string(char* str);
char bioseq_protein_dnatuple(char a, char b, char c);
int bioseq_search_string (char* str, char cmp[]);
char bioseq_protein_charge (char in);
void bioseq_protein_terminate (char* str);

bioseq bioseq_new(sequence_type type, char seq[]) {
	
	int len = strlen(seq);
	char* string = malloc((len) * sizeof(char));
	char* charge = malloc((len) * sizeof(char));
	strcpy(string, seq);
	
	bioseq_capital_string(string);
	
	bioseq new = {
		type,
		len,
		string,
		charge
	};
	
	return new;
}

void bioseq_delete(bioseq seq) {
	
	free(seq.sequence);
	seq.sequence = NULL;
	
	free(seq.charge);
	seq.charge = NULL;
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

bioseq bioseq_dna_protein(bioseq dna, int offset) {
	int remainder = dna.length % 3;
	int len = (dna.length - remainder)/3;
	char string[len+2];
	string[len] = '\0';
	
	for (int i = offset; i < len; i++) {
		
		char a = dna.sequence[i*3];
		char b = dna.sequence[i*3+1];
		char c = dna.sequence[i*3+2];
		
		string[i] = bioseq_protein_dnatuple(a, b, c);
	}
	
	return bioseq_new(SEQUENCE_PROTEIN, string);
}

void bioseq_protein_interactions(bioseq seq) {
	for (int i = 0; i < seq.length; i++) {
		seq.charge[i] = bioseq_protein_charge(seq.sequence[i]);
	}
}

bioseq bioseq_translate(bioseq seq) {
	int offset = bioseq_search_string(seq.sequence, START_CODON);
	
	bioseq out;
	if (offset >= 0) out = bioseq_dna_protein(seq, offset);

	else {
		out.sequence = "";
		out.length = -1;
		out.type = SEQUENCE_PROTEIN;
	}
	
	bioseq_protein_terminate(out.sequence);
	bioseq_protein_interactions(out);
	
	return out;
}



/*******************
Utility Functions
*******************/

char bioseq_protein_dnatuple(char a, char b, char c) {
	
	switch (a) {
		
		case 'U': case 'T':
		
			switch (b) {
				
				case 'U': case'T':
					switch (c) {
						case 'U': case 'T': case 'C':
							return 'F';  // Phenylalanine for UU[UC]
						
						case 'A': case 'G':
							return 'L'; // Leucine for UU[AG]
					}
					
				case 'C':
					return 'S'; // Serine for UCX
					
				case 'A':
					switch (c) {
						case 'A': case 'G':
							return 'X'; // Stop codon
						default: 
							return 'Y'; // Tyrosine for UA[UC] 
					}
				
				case 'G':
					switch (c) {
						case 'U': case 'T': case 'C':
							return 'C'; // Cysteine for UG[UC]
						case 'A':
							return 'X';
						case 'G':
							return 'W'; // Tryptophan for UGG	
							
					}
					
			}
			
		case 'C':
		
			switch (b) {
				
				case 'U': case'T':
					return 'L'; // Leucine for CUX
					
				case 'C':
					return 'P'; // Proline for CCX
					
				case 'A':
					switch (c) {
						case 'A': case 'G':
							return 'Q'; // Glutamine for CA[AG]
						default: 
							return 'H'; // Tyrosine for UA[UC] 
					}
				case 'G':
					return 'R'; // Arginine for CGX
			}

		case 'A':
		
			switch (b) {
				
				case 'U': case'T':
					if (c == 'G') {
						return 'M'; // Methionine for AUG ** Start Codon
					}
					return 'I'; // Isoluceine for AU[UCA]
					
				case 'C':
					return 'T'; // Threonine for ACX
					
				case 'A':
					switch (c) {
						case 'A': case 'G':
							return 'K'; // Lysine for AA[AG]
						default: 
							return 'N'; // Asparagine for AA[UC] 
					}
				case 'G':
					switch (c) {
						case 'A': case 'G':
							return 'R'; // Arginine for AG[AG]
						default:
							return 'S'; // Serine for AG[UC]
					}
			}
			
		case 'G':
		
			switch (b) {
				
				case 'U': case'T':
					return 'V'; // Valine for GUX
					
				case 'C':
					return 'A'; // Alanine for GCX
					
				case 'A':
					switch (c) {
						case 'A': case 'G':
							return 'E'; // Glutamic acid for GA[AG]
						default: 
							return 'D'; // Aspartic acid for GA[UC] 
					}
				case 'G':
					return 'G'; // Glycine for GGX
			}						
	}
	return '\0';
}

void bioseq_reverse_string(char* str) {
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

void bioseq_protein_terminate (char* str) {
	int i = 0;
	while (str[i] != '\0') {
		if (str[i] == 'X') {
			str[i] = '\0';
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

int bioseq_search_string (char* str, char cmp[]) {
	char* out = strstr(str, cmp);
	int loc = (int) (out - str);
	
	if (out) return loc;
	
	else return -1;
}

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