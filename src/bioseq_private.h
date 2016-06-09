#ifndef BIOSEQ_PRIVATE_H
#define BIOSEQ_PRIVATE_H

#include "bioseq.h"

static void bioseq_string_reverse (char* str);
static void bioseq_string_dna_complement (char* str);
static void bioseq_string_capitalize (char* str);
static void bioseq_string_excise(char* str, int index);
static char* bioseq_string_insert(char* str, int index, char ins);
static int  bioseq_string_search (char* str, char cmp[]);
static void bioseq_string_split (char* input, int index, char* output1, char* output2);
static char bioseq_protein_charge (char in);
static void bioseq_protein_terminate (char* str);
static int  bioseq_protein_verifychar (char x);
int bioseq_protein_startcodon(bioseq_protein seq);

#endif