#ifndef BIOSEQ_H
#define BIOSEQ_H

#define BUFFER_SIZE 1024
#define START_CODON "ATG"
#define END_CODON "TAG"

/***************
** Public Structs
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

typedef struct {
	long unsigned a_count;
	long unsigned c_count;
	long unsigned g_count;
	long unsigned t_count;
	long long unsigned total_count;
	float gc_percent;
	float a_percent;
	float c_percent;
	float g_percent;
	float t_percent;
} bioseq_stats;

/*****************
* Public Functions
******************/

#define NUCLEOTIDE_SANITIZE(x) (x == 'A' || x == 'C' || x == 'G' || x == 'T' || x == 'U')

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

char bioseq_dna_codon_protein(char a, char b, char c);

int bioseq_string_similarity(char* a, char* b);

void bioseq_stats_stream(bioseq_stats* stats, char in);

void bioseq_stats_calculate(bioseq_stats* stats);

#endif
