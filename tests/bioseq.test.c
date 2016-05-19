#include <tap.h>
#include <string.h>
#include "../src/bioseq.h"

#define TEST_SEQ "ATGACAGCAGCAGCGAGCGACTTCGATA"
#define TEST_SEQ_COMP "TACTGTCGTCGTCGCTCGCTGAAGCTAT"
#define TEST_SEQ_REV "ATAGCTTCAGCGAGCGACGACGACAGTA"
#define TEST_SEQ_PROT "MTAAASDFD"

static int run_tests();

int main () {
    
    // plan_no_plan();
    plan_tests(9);
    
    run_tests();
    
    return 0;
}


static char* test_dna_construct () {
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	
	ok(strcmp(test_seq.sequence, TEST_SEQ) == 0, "DNA constructor returns correct sequence.");
    ok(test_seq.length == strlen(TEST_SEQ), "DNA constructor returns correct length.");
		
	bioseq_dna_destruct(&test_seq);
	return 0;
} 

static char* test_dna_destruct () {
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna_destruct(&test_seq);
	
	ok(!test_seq.sequence, "DNA destructor pointers NULL.");
		
	return 0;
}

static char* test_protein_construct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	
	ok(strcmp(test_seq.sequence, TEST_SEQ_PROT) == 0, "Protein constructor sets correct sequence.");
    ok(test_seq.length == strlen(TEST_SEQ_PROT), "Protein constructor sets correct length.");
	
	bioseq_protein_destruct(&test_seq);
	return 0;
} 

static char* test_protein_destruct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	bioseq_protein_destruct(&test_seq);
	
	ok(!test_seq.sequence && !test_seq.charge, "Protein destructor pointers NULL.");
			
	return 0;
}

static char* test_complement () {
	
	bioseq_dna test_comp = bioseq_complement(bioseq_dna_construct(TEST_SEQ));
	
	ok1(strcmp(test_comp.sequence, TEST_SEQ_COMP) == 0);
	
	bioseq_dna_destruct(&test_comp);
	
	return 0;
}

static char* test_reverse () {
	
	bioseq_dna test_rev = bioseq_reverse(bioseq_dna_construct(TEST_SEQ));
	
	ok1(strcmp(test_rev.sequence, TEST_SEQ_REV) == 0);
	
	bioseq_dna_destruct(&test_rev);
	
	return 0;
}

static char* test_dna_protein () {
	
	// Case 1 -- Start codon
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);	
	bioseq_protein test_prot = bioseq_dna_protein(test_seq, 0);	
	
	ok1(strcmp(test_prot.sequence, TEST_SEQ_PROT) == 0);
	
	// Case 2 -- Start and Stop
	test_seq = bioseq_dna_construct("AUGGCGAGGGAGUUAUGGUGA");
	test_prot = bioseq_dna_protein(test_seq, 0);
	
	ok1(strcmp(test_prot.sequence, "MARELWX") == 0);
	
	// Case 3 -- Misaligned , uneven
	test_prot = bioseq_dna_protein(test_seq, 1);
	
	ok1(strcmp(test_prot.sequence, "WRGSYG") == 0);
	
	return 0;
}

static int run_tests() {
	
	test_dna_construct();
	test_dna_destruct();
	test_protein_construct();
	test_protein_destruct();
	test_complement();
	test_reverse();
	test_dna_protein();
	
	return 0;
}