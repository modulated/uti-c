#ifndef BIOSEQ_PRIVATE_H
#define BIOSEQ_PRIVATE_H

static void bioseq_string_reverse (char* str);
static void bioseq_string_complement (char* str);
static void bioseq_string_capital (char* str);
static void bioseq_string_excise (char* str, int index);
static char bioseq_protein_dnatuple (char a, char b, char c);
static int  bioseq_string_search (char* str, char cmp[]);
static void bioseq_string_split (char* input, int index, char* output1, char* output2);
static char bioseq_protein_charge (char in);
static void bioseq_protein_terminate (char* str);
static int  bioseq_protein_verifychar (char x);

#endif