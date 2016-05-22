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
	int viable_sequences;
	bioseq_protein frames[6];
} bioseq_frame;


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

bioseq_protein bioseq_dna_translate(bioseq_dna dna, int offset);

bioseq_protein bioseq_dna_translatefromstartcodon(bioseq_dna dna);

void bioseq_protein_interactions(bioseq_protein seq);

void bioseq_dna_split(bioseq_dna seq, int index, bioseq_dna* out1, bioseq_dna* out2);

void bioseq_protein_split(bioseq_protein seq, int index, bioseq_protein* out1, bioseq_protein* out2);

bioseq_frame bioseq_frame_construct(bioseq_dna seq);

void bioseq_frame_destruct(bioseq_frame* frame);

int bioseq_frame_getopen(bioseq_frame frame);

#endif
