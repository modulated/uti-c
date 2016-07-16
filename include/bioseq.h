#ifndef _BIOSEQ_H
#define _BIOSEQ_H

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

/*
** DNA Functions
*/

#define NUCLEOTIDE_SANITIZE(x) (x == 'A' || x == 'C' || x == 'G' || x == 'T' || x == 'U')

bioseq_dna bioseq_dna_construct (char seq[]);

void bioseq_dna_destruct (bioseq_dna* seq);

void bioseq_dna_sanitize (bioseq_dna seq);

bioseq_dna bioseq_dna_reverse (bioseq_dna seq);

bioseq_dna bioseq_dna_complement (bioseq_dna seq);

void bioseq_dna_split (bioseq_dna seq, unsigned int index, bioseq_dna* out1, bioseq_dna* out2);

bioseq_dna bioseq_dna_substitution(bioseq_dna seq, unsigned int index, char sub);

bioseq_dna bioseq_dna_deletion(bioseq_dna seq, unsigned int index);

bioseq_dna bioseq_dna_insertion(bioseq_dna seq, unsigned int index, char sub);

bioseq_dna bioseq_dna_insertion_block(bioseq_dna seq, unsigned int index, char* sub);

bioseq_dna bioseq_dna_join (bioseq_dna start, bioseq_dna end);

bioseq_protein bioseq_dna_protein (bioseq_dna dna, unsigned int offset);

bioseq_protein bioseq_dna_translate (bioseq_dna dna, unsigned int offset);

bioseq_protein bioseq_dna_translatefromstartcodon (bioseq_dna dna);

/*
** Protein Functions
*/

bioseq_protein bioseq_protein_construct (char seq[]);

void bioseq_protein_destruct (bioseq_protein* seq);

void bioseq_protein_sanitize (bioseq_protein seq);

void bioseq_protein_interactions (bioseq_protein seq);

void bioseq_protein_split (bioseq_protein seq, unsigned int index, bioseq_protein* out1, bioseq_protein* out2);

bioseq_protein bioseq_protein_join (bioseq_protein start, bioseq_protein end);

/*
** Frame Functions
*/

bioseq_frame bioseq_frame_construct (bioseq_dna seq);

void bioseq_frame_destruct (bioseq_frame* frame);

int bioseq_frame_getopen (bioseq_frame frame);

/*
** Misc - String, stats and codon
*/

char bioseq_dna_codon_protein (char a, char b, char c);

int bioseq_string_similarity (char* a, char* b);

int bioseq_string_insertions (char* a, char* b);

int bioseq_string_deletions (char* a, char* b);

void bioseq_stats_stream (bioseq_stats* stats, char in);

void bioseq_stats_calculate (bioseq_stats* stats);

// Ex Private
void bioseq_string_reverse (char* str);
void bioseq_string_dna_complement (char* str);
void bioseq_string_capitalize (char* str);
void bioseq_string_excise(char* str, int index);
char* bioseq_string_insert(char* str, int index, char ins);
int  bioseq_string_search (char* str, char cmp[]);
void bioseq_string_split (char* input, int index, char* output1, char* output2);
char* bioseq_string_insert_chunk(char* str, int index, char* ins); 
char bioseq_protein_charge (char in);
void bioseq_protein_terminate (char* str);
int  bioseq_protein_verifychar (char x);
int bioseq_protein_startcodon(bioseq_protein seq);

#endif
