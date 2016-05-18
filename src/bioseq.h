#ifndef _BIOSEQ_H

#define _BIOSEQ_H


typedef struct {
	unsigned length;
	char* sequence;
} bioseq_dna;

typedef struct {
	unsigned length;
	char* sequence;
	char* charge;
} bioseq_protein;

typedef struct {
	unsigned length;
	bioseq_protein* list[];
} bioseq_orf;

bioseq_dna bioseq_dna_construct(char seq[]);

void bioseq_dna_destruct(bioseq_dna* seq);

bioseq_protein bioseq_protein_construct(char seq[]);

void bioseq_protein_destruct(bioseq_protein* seq);

bioseq_dna bioseq_reverse(bioseq_dna seq);

bioseq_dna bioseq_complement(bioseq_dna seq);

bioseq_protein bioseq_dna_protein(bioseq_dna dna, int offset);

void bioseq_protein_interactions(bioseq_protein seq);

#endif