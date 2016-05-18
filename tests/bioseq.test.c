#include "test.h"
#include "../src/bioseq.c"
#include "../src/color.h"

#define TEST_SEQ "ATGACAGCAGCAGCGAGCGACTTCGATA"
#define TEST_SEQ_COMP "TACTGTCGTCGTCGCTCGCTGAAGCTAT"
#define TEST_SEQ_REV "ATAGCTTCAGCGAGCGACGACGACAGTA"
#define TEST_SEQ_PROT "MTAAASDFD"

int test_count = 0;



static char* test_dna_construct () {
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	
	test_assert(KRED"ERROR: bioseq_dna_construct does not return appropriate struct"RESET, strcmp(test_seq.sequence, TEST_SEQ) == 0);
	puts("bioseq_dna_construct \t "KGRN"PASSED"RESET);
	
	bioseq_dna_destruct(&test_seq);
	return 0;
} 

static char* test_dna_destruct () {
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna_destruct(&test_seq);
	
	test_assert(KRED"ERROR: bioseq_dna_destruct does not NULL pointers"RESET, !test_seq.sequence);
	
	puts("bioseq_dna_destruct \t "KGRN"PASSED"RESET);	
	return 0;
}

static char* test_protein_construct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	
	test_assert(KRED"ERROR: bioseq_protein_construct does not return appropriate struct"RESET, strcmp(test_seq.sequence, TEST_SEQ_PROT) == 0);
	puts("bioseq_protein_construct \t "KGRN"PASSED"RESET);
	
	bioseq_protein_destruct(&test_seq);
	return 0;
} 

static char* test_protein_destruct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	bioseq_protein_destruct(&test_seq);
	
	test_assert(KRED"ERROR: bioseq_protein_destruct does not NULL pointers"RESET, !test_seq.sequence && !test_seq.charge);
	
	puts("bioseq_protien_destruct \t "KGRN"PASSED"RESET);	
	return 0;
}

static char* test_complement () {
	
	bioseq_dna test_comp = bioseq_complement(bioseq_dna_construct(TEST_SEQ));
	
	test_assert(KRED"ERROR: bioseq_complement does not return expected value"RESET, strcmp(test_comp.sequence, TEST_SEQ_COMP) == 0);
	
	bioseq_dna_destruct(&test_comp);
	
	puts("bioseq_complement \t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_reverse () {
	
	bioseq_dna test_rev = bioseq_reverse(bioseq_dna_construct(TEST_SEQ));
	
	test_assert(KRED"ERROR: bioseq_reverse does not return expected value"RESET, strcmp(test_rev.sequence, TEST_SEQ_REV) == 0);
	
	bioseq_dna_destruct(&test_rev);
	
	puts("bioseq_reverse \t\t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_protein_terminate () {
	
	char test_terminate[] = "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG";
	char pass_terminate[] = "THE QUICK BROWN FO";
	
	bioseq_protein_terminate(test_terminate);
	
	test_assert(KRED"ERROR: bioseq_protein_terminate does not return expected value"RESET, strcmp(test_terminate, pass_terminate) == 0);
	
	puts("bioseq_protein_terminate "KGRN"PASSED"RESET);
	return 0;
}

static char* test_search_string () {
	
	// Case 1 -- find phrase
	char test_search[] = "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG";
	char* term_search = strdup("BROWN");
	
	int res_search = bioseq_search_string(test_search, term_search);
	int pass_search = 10;
	
	test_assert(KRED"ERROR: bioseq_search_string does not return expected value"RESET, res_search == pass_search);
	
	// Case 2 -- no phrase found
	free(term_search);
	term_search = strdup("JUMPED ON");
	
	res_search = bioseq_search_string(test_search, term_search);
	pass_search = -1;
	
	test_assert(KRED"ERROR: bioseq_search_string does not return expected value"RESET, res_search == pass_search);
	
	free(term_search);
	
	puts("bioseq_search_string \t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_dna_tuple () {
	
	// Case 1 -- normal tuple
	
	char test_out = bioseq_protein_dnatuple('A', 'T', 'G');
	char pass_out = 'M';
		
	test_assert(KRED"ERROR: bioseq_dna_tuple does not return expected value"RESET, test_out == pass_out);
	
	test_out = bioseq_protein_dnatuple('T', 'A', 'T');
	pass_out = 'Y';
		
	test_assert(KRED"ERROR: bioseq_dna_tuple does not return expected value"RESET, test_out == pass_out);
	
	// Case 2 -- corrupt tuple

	test_out = bioseq_protein_dnatuple('a', 'e', ' ');
	pass_out = '?';
	
	test_assert(KRED"ERROR: bioseq_dna_tuple does not return expected value"RESET, test_out == pass_out);

	
	puts("bioseq_dna_tuple \t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_dna_protein () {
	
	// Case 1 -- Start codon
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);	
	bioseq_protein test_prot = bioseq_dna_protein(test_seq, 0);	
	
	test_assert(KRED"ERROR: bioseq_dna_protein does not return expected value"RESET, strcmp(test_prot.sequence, TEST_SEQ_PROT) == 0);
	
	// Case 2 -- Start and Stop
	test_seq = bioseq_dna_construct("AUGGCGAGGGAGUUAUGGUGA");
	test_prot = bioseq_dna_protein(test_seq, 0);
	
	test_assert(KRED"ERROR: bioseq_dna_protein does not return expected value"RESET, strcmp(test_prot.sequence, "MARELWX") == 0);
	
	// Case 3 -- Misaligned , uneven
	test_prot = bioseq_dna_protein(test_seq, 1);
	
	test_assert(KRED"ERROR: bioseq_dna_protein does not return expected value"RESET, strcmp(test_prot.sequence, "WRGSYG") == 0);
	
	puts("bioseq_dna_protein \t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_all() {
	
	test_run(test_dna_construct);
	test_run(test_dna_destruct);
	test_run(test_protein_construct);
	test_run(test_protein_destruct);
	test_run(test_complement);
	test_run(test_reverse);
	test_run(test_protein_terminate);
	test_run(test_search_string);
	test_run(test_dna_tuple);
	test_run(test_dna_protein);
	
	return 0;
}

int main() {
	
	char* result = test_all();
	printf("Tests run: "KBLU"%d\n"RESET, test_count);
	
	if (result !=0) {
		printf("%s\n", result);
	}
	
	else {
		puts(KGRN"\nALL TESTS PASSED"RESET);
	}
	
	
	return result != 0;
}
