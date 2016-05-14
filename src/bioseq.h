#ifndef _BIOSEQ_H

#define _BIOSEQ_H

typedef enum SEQUENCE_TYPE {
	SEQUENCE_NONE,
	SEQUENCE_DNA,
	SEQUENCE_RNA,
	SEQUENCE_PROTEIN
} sequence_type;

typedef struct {
	sequence_type type;
	unsigned length;
	char* sequence;
	
} bioseq;

bioseq bioseq_new(sequence_type type, char seq[]);

void bioseq_delete(bioseq seq);

bioseq bioseq_reverse(bioseq seq);

bioseq bioseq_complement(bioseq seq);

#endif