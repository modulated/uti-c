#include <tap.h>
#include <string.h>
#include "../src/bioseq.h"

#define E_COLI "ATGAAGAAATCAATATTATTTATTTTTCTTTCTGTATTGTCTTTTTCACCTTTCGCTCAGGATGCTAAACCAGTAGAGTCTTCAAAAGAAAAAATCACACTAGAATCAAAAAAATGTAACATTGCAAAAAAAAGTAATAAAAGTGGTCCTGAAAGCATGAATAGTAGCAATTACTGCTGTGAATTGTGTTGTAATCCTGCTTGTACCGGGTGCTATTAA"

#define TEST_SEQ "ATGACAGCAGCAGCGAGCGACTTCGATA"
#define TEST_SEQ_SANITIZE " AT GACAGCAD GCAGCGAGCGACT1 TCGATA"
#define TEST_SEQ_COMP "TACTGTCGTCGTCGCTCGCTGAAGCTAT"
#define TEST_SEQ_REV "ATAGCTTCAGCGAGCGACGACGACAGTA"
#define TEST_SEQ_PROT "MTAAASDFD"
#define TEST_SEQ_PROT_SANITIZE " # M TB AAASDFD 1"

void run_tests();

int main () {
    
    plan_no_plan();
    // plan_tests(14);
    
    run_tests();
    
    return 0;
}


void test_dna_construct () {
	bioseq_dna test_seq = bioseq_dna_construct(E_COLI);
	
	ok(strcmp(test_seq.sequence, E_COLI) == 0, "DNA constructor returns correct sequence.");
    ok(test_seq.length == strlen(E_COLI), "DNA constructor returns correct length.");
		
	bioseq_dna_destruct(&test_seq);
} 

void test_dna_destruct () {
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna_destruct(&test_seq);
	
	ok(!test_seq.sequence, "DNA destructor pointers NULL.");
		
}

void test_protein_construct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	
	ok(strcmp(test_seq.sequence, TEST_SEQ_PROT) == 0, "Protein constructor sets correct sequence.");
    ok(test_seq.length == strlen(TEST_SEQ_PROT), "Protein constructor sets correct length.");
	
	bioseq_protein_destruct(&test_seq);
} 

void test_protein_destruct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	bioseq_protein_destruct(&test_seq);
	
	ok(!test_seq.sequence && !test_seq.charge, "Protein destructor pointers NULL.");			
}

void test_dna_sanitize () {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ_SANITIZE);
	ok(strcmp(test_seq.sequence, TEST_SEQ) == 0, "DNA input sanitized correctly.");
	
	bioseq_dna_destruct(&test_seq);			
}

void test_protein_sanitize () {
	
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT_SANITIZE);
	ok(strcmp(test_seq.sequence, TEST_SEQ_PROT) == 0, "Protein input sanitized correctly. Result: %s", test_seq.sequence);
	
	bioseq_protein_destruct(&test_seq);			
}

void test_dna_complement () {
	
	bioseq_dna test_comp = bioseq_dna_complement(bioseq_dna_construct(TEST_SEQ));
	
	ok1(strcmp(test_comp.sequence, TEST_SEQ_COMP) == 0);
	
	bioseq_dna_destruct(&test_comp);	
}

void test_dna_reverse () {
	
	bioseq_dna test_rev = bioseq_dna_reverse(bioseq_dna_construct(TEST_SEQ));
	
	ok1(strcmp(test_rev.sequence, TEST_SEQ_REV) == 0);
	
	bioseq_dna_destruct(&test_rev);	
}

void test_dna_protein () {
	
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
	
	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
}

void test_dna_split () {
	
	bioseq_dna test_seq = bioseq_dna_construct("ATGATG");
	bioseq_dna res_seq1;
	bioseq_dna res_seq2;

	
	
	// Case 1 -- Even
	int index = 2;
	char res_str1[] = "AT";
	char res_str2[] = "GATG";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str1, res_seq1.sequence) == 0, "%s == %s", res_str1, res_seq1.sequence);
	ok(strcmp(res_str2, res_seq2.sequence) == 0, "%s == %s", res_str2, res_seq2.sequence);
	
	
	// Case 2 -- Odd
	index = 5;
	char res_str3[] = "ATGAT";
	char res_str4[] = "G";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str3, res_seq1.sequence) == 0, "%s == %s", res_str3, res_seq1.sequence);
	ok(strcmp(res_str4, res_seq2.sequence) == 0, "%s == %s", res_str4, res_seq2.sequence);

	// Case 3 -- Zero
	index = 0;
	char res_str5[] = "";
	char res_str6[] = "ATGATG";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str5, res_seq1.sequence) == 0, "%s == %s", res_str5, res_seq1.sequence);
	ok(strcmp(res_str6, res_seq2.sequence) == 0, "%s == %s", res_str6, res_seq2.sequence);

	// Case 2 -- Overflow
	index = 9;
	char res_str7[] = "ATGATG";
	char res_str8[] = "";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str7, res_seq1.sequence) == 0, "%s == %s", res_str7, res_seq1.sequence);
	ok(strcmp(res_str8, res_seq2.sequence) == 0, "%s == %s", res_str8, res_seq2.sequence);

	// Case 2 -- Negative
	index = -3;
	char res_str9[] = "ATGATG";
	char res_str10[] = "";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str9, res_seq1.sequence) == 0, "%s == %s", res_str9, res_seq1.sequence);
	ok(strcmp(res_str10, res_seq2.sequence) == 0, "%s == %s", res_str10, res_seq2.sequence);	

	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
}

void run_tests () {
	
	test_dna_construct();
	test_dna_destruct();
	test_protein_construct();
	test_protein_destruct();
	test_dna_sanitize();
	test_protein_sanitize();
	test_dna_complement();
	test_dna_reverse();
	test_dna_protein();
	test_dna_split();
}