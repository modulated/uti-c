#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bioseq.h"

#define BUFFER_SIZE 1024
#define START_CODON "ATG"
#define NUCLEOTIDE_SANITIZE(x) (x == 'A' || x == 'C' || x == 'G' || x == 'T' || x == 'U')

/*******************
** Private Functions
********************/

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

static void bioseq_complement_string(char* str) {
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

static void bioseq_protein_terminate (char* str) {
	int i = 0;
	while (str[i++]) {
		if (str[i] == 'X') {
			str[i] = '\0';
			break;
		}
	}
	
}

static void bioseq_string_capital (char* str) {
	int i = 0;
	while (str[i]) {
		str[i] = toupper(str[i]);
		i++;
	}
}

// Returns 1 if char matches single letter protein code or 0 if not. 
static int bioseq_protein_verifychar(char x) {
	
	switch(x) {
		case 'A': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'L': case 'K': case 'M': case 'N': case 'Q': case 'P': case 'R': case 'S': case 'T': case 'V': case 'W': case 'Y': case 'X':
			return 1;			
	}
	return 0;
}

// Searches string for phrase and returns location in string, or -1 if not found 
static int bioseq_string_search (char* str, char cmp[]) {
	char* out = strstr(str, cmp);
	int loc = (int) (out - str);
	
	if (out) return loc;
	
	else return -1;
}

/* Returns protein charge for given single letter code.
+ = positive, - = negative, # = hydrophobic, * = hydrophilic */
static char bioseq_protein_charge (char in) {
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


/*******************
** Public Functions
********************/

bioseq_dna bioseq_dna_construct(char seq[]) {
	
	
	int len = strlen(seq);	
	
	bioseq_dna new = {
		len,
		strdup(seq)		
	};
	
	bioseq_string_capital(new.sequence);	
	bioseq_dna_sanitize(new);
	new.length = strlen(new.sequence);
		
	return new;
}

void bioseq_dna_destruct(bioseq_dna* seq) {
	// Free allocated heap and set pointers to NULL	
	
	free(seq->sequence);
	seq->sequence = NULL;
}

bioseq_protein bioseq_protein_construct(char seq[]) {
	
	int len = strlen(seq);
	char* charge = malloc(len * sizeof(char));
	
	bioseq_protein new = {
		len,
		strdup(seq),
		charge
	};
	
	bioseq_string_capital(new.sequence);	
	bioseq_protein_sanitize(new);
	new.length = strlen(seq);
	
	return new;
}

void bioseq_protein_destruct(bioseq_protein* seq) {
	// Free allocated heap and set pointers to NULL	
	
	free(seq->sequence);
	seq->sequence = NULL;
	
	free(seq->charge);
	seq->charge = NULL;
}

bioseq_dna bioseq_dna_reverse(bioseq_dna seq) {
	
	int len = seq.length;
	char string[len];
	strcpy(string, seq.sequence);
	
	bioseq_string_reverse(string);
	
	return bioseq_dna_construct(string);
}

void bioseq_dna_sanitize(bioseq_dna seq) {
	
	char* str = seq.sequence;
	int i = 0;
	
	while (str[i]) {
		
		if (!NUCLEOTIDE_SANITIZE(str[i])) {
					
			bioseq_string_excise(str, i);			
		}
		
		else {			
			i++;
		}
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

// Convert 3 char codon to single char protein code. (X = stop codon)
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
							
						default:
							return '?';
					}
					
				case 'C':
					if (NUCLEOTIDE_SANITIZE(c)) {					
						return 'S'; // Serine for UCX
					}
					
					else {
						return '?';
					}
					
				case 'A':
					switch (c) {
						case 'A': case 'G':
							return 'X'; // Stop codon
						
						case 'T': case 'U': case 'C': 
							return 'Y'; // Tyrosine for UA[UC]
						
						default:
							return '?'; 
					}
				
				case 'G':
					switch (c) {
						case 'U': case 'T': case 'C':
							return 'C'; // Cysteine for UG[UC]
						case 'A':
							return 'X';
						case 'G':
							return 'W'; // Tryptophan for UGG
							
						default:
							return '?';
							
					}
					
				default:
					return '?';					
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
						
						case 'U': case 'T': case 'C': 
							return 'H'; // Tyrosine for UA[UC]
						
						default:
							return '?';
					}
				case 'G':
					return 'R'; // Arginine for CGX
					
				default:
					return '?';
			}

		case 'A':
		
			switch (b) {
				
				case 'U': case'T':
					if (c == 'G') {
						return 'M'; // Methionine for AUG ** Start Codon
					}
					
					else if (c == 'A' || c == 'C' || c == 'T' || c == 'U') {						
						return 'I'; // Isoluceine for AU[UCA]
					}
					
					else {
						return '?';
					}
					
				case 'C':
					if (NUCLEOTIDE_SANITIZE(b)) {
						return 'T'; // Threonine for ACX
					}
					
					else {
						return '?';
					}
					
				case 'A':
					switch (c) {
						case 'A': case 'G':
							return 'K'; // Lysine for AA[AG]
							
						case 'T': case 'U': case 'C': 
							return 'N'; // Asparagine for AA[UC]
							
						default:
							return '?';
					}
				case 'G':
					switch (c) {
						case 'A': case 'G':
							return 'R'; // Arginine for AG[AG]
							
						case 'C': case 'T': case 'U':
							return 'S'; // Serine for AG[UC]
							
						default:
							return '?';
					}
				
				default:
					return '?';
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
							
						case 'C': case 'T': case 'U': 
							return 'D'; // Aspartic acid for GA[UC]
						
						default:
							return '?';
					}
				case 'G':
					return 'G'; // Glycine for GGX
					
				default:
					return '?';
			}
		
		default:
			return '?';						
	}
	return '?';
}

bioseq_dna bioseq_dna_complement(bioseq_dna seq) {
	
	int len = seq.length;
	char string[len];
	strcpy(string, seq.sequence);
	
	bioseq_complement_string(string);
	
	return bioseq_dna_construct(string);
	
}

void bioseq_dna_split(bioseq_dna seq, int index, bioseq_dna* out1, bioseq_dna* out2) {
	
	if (index < 0) index = 0;
	if (index > seq.length) index = seq.length;
	
	char* str1 = malloc((index + 1) * sizeof(char));
	char* str2 = malloc((seq.length - index + 1) * sizeof(char)); 
	bioseq_string_split(seq.sequence, index, str1, str2);
	
	*out1 = bioseq_dna_construct(str1);
	*out2 = bioseq_dna_construct(str2);
	
	free(str1);
	free(str2);
}

void bioseq_protein_split(bioseq_protein seq, int index, bioseq_protein* out1, bioseq_protein* out2) {
	
	if (index < 0) index = 0;
	if (index > seq.length) index = seq.length;
	
	char* str1 = malloc((index + 1) * sizeof(char));
	char* str2 = malloc((seq.length - index + 1) * sizeof(char)); 
	bioseq_string_split(seq.sequence, index, str1, str2);
	
	*out1 = bioseq_protein_construct(str1);
	*out2 = bioseq_protein_construct(str2);
	
	free(str1);
	free(str2);	
}

bioseq_protein bioseq_dna_protein(bioseq_dna dna, int offset) {
	
	int seqlength = dna.length - offset;
	int rem = seqlength % 3;
	int len = (seqlength - rem)/3;
	char string[len+1];
	
	for (int i = 0; i < len; i++) {
		
		char a = dna.sequence[i * 3 + offset];
		char b = dna.sequence[i * 3 + 1 + offset];
		char c = dna.sequence[i * 3 + 2 + offset];
		
		string[i] = bioseq_protein_dnatuple(a, b, c);
	}
	
	string[len] = '\0';
	
	return bioseq_protein_construct(string);
}

void bioseq_protein_interactions(bioseq_protein seq) {
	
	for (int i = 0; i < seq.length; i++) {
		seq.charge[i] = bioseq_protein_charge(seq.sequence[i]);
	}
}

bioseq_protein bioseq_dna_translate(bioseq_dna seq, int offset) {
	
	bioseq_protein out;
	out = bioseq_dna_protein(seq, offset);
	
	if (bioseq_string_search(out.sequence, "M")) {
		bioseq_protein_terminate(out.sequence);
	}
	
	else {
		out.sequence = "";
		out.length = 0;
	}
	
	bioseq_protein_terminate(out.sequence);
	bioseq_protein_interactions(out);
	
	return out;
}

bioseq_protein bioseq_dna_translatefromstartcodon(bioseq_dna seq) {
	
	int offset = bioseq_string_search(seq.sequence, START_CODON);
	
	bioseq_protein out;
	if (offset >= 0) out = bioseq_dna_protein(seq, offset);

	else {
		out.sequence = "";
		out.length = 0;
	}
	
	bioseq_protein_terminate(out.sequence);
	bioseq_protein_interactions(out);
	
	return out;
}

bioseq_frame bioseq_frame_construct(bioseq_dna seq) {
	
	bioseq_frame out;
	
	for (int i = 0; i < 3; i++) {
		out.frames[i] = bioseq_dna_protein(seq, i);		
	}
	
	seq = bioseq_dna_reverse(seq);
	seq = bioseq_dna_complement(seq);
	
	for (int i = 0; i < 3; i++) {
		out.frames[i+3] = bioseq_dna_protein(seq, i);
	}		
	
	return out;
}

void bioseq_frame_destruct(bioseq_frame* frame) {
	for (int i = 0; i < 6; i++) {
		bioseq_protein_destruct(&frame->frames[i]);
	}
}

int bioseq_frame_getopen(bioseq_frame frame) {
	
	int count = 0;
	
	for (int i = 0; i < 6; i++) {
		if (frame.frames[i].length == 0) i++;
		else count++;
	}
	
	return count;
}
