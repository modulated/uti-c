/* bioseq_util.c
** contains utility functions 
*/

#include "bioseq.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


void bioseq_stats_stream(bioseq_stats* stats, char in) {
	
	switch (in) {
		case 'A': stats->a_count++; break;
		case 'C': stats->c_count++; break;
		case 'G': stats->g_count++; break;
		case 'T': stats->t_count++; break;
		default: printf("Error char: %c", in); exit(1);
	}
	
	stats->total_count++;
}



void bioseq_stats_calculate(bioseq_stats* stats) {
	
	stats->a_percent = (float) stats->a_count / (float) stats->total_count * 100;
	stats->c_percent = (float) stats->c_count / (float) stats->total_count * 100;
	stats->g_percent = (float) stats->g_count / (float) stats->total_count * 100;
	stats->t_percent = (float) stats->t_count / (float) stats->total_count * 100;
	
	stats->gc_percent = (float) (stats->c_count + stats->g_count) / (float) stats->total_count * 100;
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
		bioseq_protein_destruct(&(frame->frames[i]));
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


// Convert 3 char codon to single char protein code. (X = stop codon)
char bioseq_dna_codon_protein(char a, char b, char c) {

	a = toupper(a);
	b = toupper(b);
	c = toupper(c);
	
	if (!NUCLEOTIDE_SANITIZE(a) || !NUCLEOTIDE_SANITIZE(b) || !NUCLEOTIDE_SANITIZE(c)) return '?';
	
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
					return 'S'; // Serine for UCX					
					
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

					}
				case 'G':
					if (NUCLEOTIDE_SANITIZE(c)) return 'G'; // Glycine for GGX
					else return '?';
					
				default:
					return '?';
			}
		
		default:
			return '?';						
	}
	return '?';
}