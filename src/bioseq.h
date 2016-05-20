#ifndef BIOSEQ_H
#define BIOSEQ_H


/***************
* Public Structs
****************/

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


/*****************
* Public Functions
******************/

bioseq_dna bioseq_dna_construct(char seq[]);

void bioseq_dna_destruct(bioseq_dna* seq);

bioseq_protein bioseq_protein_construct(char seq[]);

void bioseq_protein_destruct(bioseq_protein* seq);

void bioseq_dna_sanitize(bioseq_dna seq);

void bioseq_protein_sanitize(bioseq_protein seq);

bioseq_dna bioseq_dna_reverse(bioseq_dna seq);

bioseq_dna bioseq_dna_complement(bioseq_dna seq);

bioseq_protein bioseq_dna_protein(bioseq_dna dna, int offset);

void bioseq_protein_interactions(bioseq_protein seq);

void bioseq_dna_split(bioseq_dna seq, int position, bioseq_dna* out1, bioseq_dna* out2);

#endif