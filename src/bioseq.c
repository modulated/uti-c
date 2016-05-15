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

bioseq bioseq_translate(bioseq seq) {
	int offset = bioseq_search_string(seq.sequence, START_CODON);
	
	bioseq out;
	if (offset >= 0) out = bioseq_dna_protein(seq, offset);
	
	else {
		out.sequence = "";
		out.length = -1;
		out.type = SEQUENCE_PROTEIN;
	}
	
	return out;
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
	
	bioseq prot = bioseq_translate(test);
	printf("Protein: %s \tLen: %u\n", prot.sequence, prot.length);
	
	bioseq_delete(test);
	
	return 0;
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