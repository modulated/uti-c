/* bioseq_dna.c
** contains functions that manipulate dna sequences
*/

#include "bioseq.h"
#include "bioseq_private.h"
#include <stdlib.h>
#include <string.h>


bioseq_dna bioseq_dna_construct(char seq[]) {
	
	
	int len = strlen(seq);	
	
	bioseq_dna new = {
		len,
		strdup(seq)		
	};
	
	bioseq_string_capitalize(new.sequence);	
	bioseq_dna_sanitize(new);
	new.length = strlen(new.sequence);
		
	return new;
}



void bioseq_dna_destruct(bioseq_dna* seq) {
	// Free allocated heap and set pointers to NULL	
	
	free(seq->sequence);
	seq->sequence = NULL;
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



bioseq_dna bioseq_dna_complement(bioseq_dna seq) {
	
	int len = seq.length;
	char string[len];
	strcpy(string, seq.sequence);
	
	bioseq_string_dna_complement(string);
	
	return bioseq_dna_construct(string);
	
}


// Splits seq into out1 and out2 at given index. Need to preallocate out structs.
void bioseq_dna_split(bioseq_dna seq, int index, bioseq_dna* out1, bioseq_dna* out2) {
	
	if (index < 0) index = 0;	
	
	char* str1 = malloc((index + 1) * sizeof(char));
	char* str2 = malloc((seq.length - index + 1) * sizeof(char)); 
	bioseq_string_split(seq.sequence, index, str1, str2);
	
	*out1 = bioseq_dna_construct(str1);
	*out2 = bioseq_dna_construct(str2);
	
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
		
		string[i] = bioseq_dna_codon_protein(a, b, c);
	}
	
	string[len] = '\0';
	
	return bioseq_protein_construct(string);
}



bioseq_protein bioseq_dna_translate(bioseq_dna seq, int offset) {
	
	bioseq_protein out;
	out = bioseq_dna_protein(seq, offset);
	
	if (bioseq_protein_startcodon(out)) {
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



bioseq_protein bioseq_dna_translate_s2s(bioseq_dna seq) {
	
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