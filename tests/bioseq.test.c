#include "tap.h"
#include <string.h>
#include "../src/bioseq.h"
#include <stdio.h>

#define E_COLI "ATGAAGAAATCAATATTATTTATTTTTCTTTCTGTATTGTCTTTTTCACCTTTCGCTCAGGATGCTAAACCAGTAGAGTCTTCAAAAGAAAAAATCACACTAGAATCAAAAAAATGTAACATTGCAAAAAAAAGTAATAAAAGTGGTCCTGAAAGCATGAATAGTAGCAATTACTGCTGTGAATTGTGTTGTAATCCTGCTTGTACCGGGTGCTATTAA"

#define EMPTY_STRING ""
#define TEST_SEQ "ATGACAGCAGCAGCGAGCGACTTCGATA"
#define TEST_SEQ_SANITIZE " AT GACAGCAD GCAGCGAGCGACT1 TCGATA"
#define TEST_SEQ_COMP "TACTGTCGTCGTCGCTCGCTGAAGCTAT"
#define TEST_SEQ_REV "ATAGCTTCAGCGAGCGACGACGACAGTA"
#define TEST_SEQ_PROT "MTAAASDFD"
#define TEST_SEQ_PROT_2 "XQQQRATSI"
#define TEST_SEQ_PROT_3 "DSSSERLR"
#define TEST_SEQ_PROT_4 "YRSRSLLLS"
#define TEST_SEQ_PROT_5 "IEVARCCCH"
#define TEST_SEQ_PROT_6 "SKSLAAAV"
#define TEST_SEQ_PROT_SANITIZE " # M TB AAASDFD 1"

void run_tests();




void test_dna_construct () {

	// Case 1 - normal use
	bioseq_dna test_seq = bioseq_dna_construct(E_COLI);
	
	ok(strcmp(test_seq.sequence, E_COLI) == 0, "DNA constructor returns correct sequence.");
    ok(test_seq.length == strlen(E_COLI), "DNA constructor returns correct length.");
		
	bioseq_dna_destruct(&test_seq);


	// Case 2 - empty string
	test_seq = bioseq_dna_construct("");
	
	ok(strcmp(test_seq.sequence, "") == 0, "DNA constructor returns correct sequence.");
    ok(test_seq.length == 0, "DNA constructor returns correct length.");
		
	bioseq_dna_destruct(&test_seq);


	// Case 2 - NULL string
	test_seq = bioseq_dna_construct(NULL);
	
	ok(strcmp(test_seq.sequence, "") == 0, "DNA constructor returns correct sequence.");
    ok(test_seq.length == 0, "DNA constructor returns correct length.");
		
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

	// Case 2 - empty string
	test_seq = bioseq_protein_construct("");
	
	ok(strcmp(test_seq.sequence, "") == 0, "Protein constructor returns correct sequence.");
    ok(test_seq.length == 0, "Protein constructor returns correct length.");
		
	bioseq_protein_destruct(&test_seq);


	// Case 2 - NULL string
	test_seq = bioseq_protein_construct(NULL);
	
	ok(strcmp(test_seq.sequence, "") == 0, "Protein constructor returns correct sequence.");
    ok(test_seq.length == 0, "Protein constructor returns correct length.");
		
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
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna test_comp = bioseq_dna_complement(test_seq);
	
	ok1(strcmp(test_comp.sequence, TEST_SEQ_COMP) == 0);
	
	bioseq_dna_destruct(&test_comp);
	bioseq_dna_destruct(&test_seq);	
}

void test_dna_reverse () {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna test_rev = bioseq_dna_reverse(test_seq);
	
	ok1(strcmp(test_rev.sequence, TEST_SEQ_REV) == 0);
	
	bioseq_dna_destruct(&test_rev);
	bioseq_dna_destruct(&test_seq);
	
}

void test_dna_protein () {
	
	// Case 1 -- Start codon
	int test_int = 0;
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);	
	bioseq_protein test_prot = bioseq_dna_protein(test_seq, 0);	
	
	test_int = test_int || !(strcmp(test_prot.sequence, TEST_SEQ_PROT) == 0);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
		
	// Case 2 -- Start and Stop
	test_seq = bioseq_dna_construct("AUGGCGAGGGAGUUAUGGUGA");
	test_prot = bioseq_dna_protein(test_seq, 0);
	
	test_int = test_int || !(strcmp(test_prot.sequence, "MARELWX") == 0);

	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
	
	// Case 3 -- Misaligned , uneven
	test_seq = bioseq_dna_construct("AUGGCGAGGGAGUUAUGGUGA");
	test_prot = bioseq_dna_protein(test_seq, 1);
	
	test_int = test_int || !(strcmp(test_prot.sequence, "WRGSYG") == 0);
	
	ok(test_int == 0, "dna_protein returns expected values.");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
}

void test_dna_join () {
	// Case 1 - normal values
	bioseq_dna test_seq = bioseq_dna_construct("ATGCTA");
	bioseq_dna test_seq2 = bioseq_dna_construct("GCATA");
	char res_string[] = "ATGCTAGCATA";
		
	bioseq_dna res_seq = bioseq_dna_join(test_seq, test_seq2);
	
	ok(strcmp(res_seq.sequence, res_string) == 0, "dna_join returns appropriate sequence");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&test_seq2);
	bioseq_dna_destruct(&res_seq);

	// Case 2 - null values

	test_seq = bioseq_dna_construct(NULL);
	test_seq2 = bioseq_dna_construct(NULL);
	char res_string2[] = "";
		
	res_seq = bioseq_dna_join(test_seq, test_seq2);
	
	ok(strcmp(res_seq.sequence, res_string2) == 0, "dna_join returns appropriate sequence");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&test_seq2);
	bioseq_dna_destruct(&res_seq);

	// Case 3 - single null value

	test_seq = bioseq_dna_construct(NULL);
	test_seq2 = bioseq_dna_construct("AGATAG");
	char res_string3[] = "AGATAG";
		
	res_seq = bioseq_dna_join(test_seq, test_seq2);
	
	ok(strcmp(res_seq.sequence, res_string3) == 0, "dna_join returns appropriate sequence");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&test_seq2);
	bioseq_dna_destruct(&res_seq);
}

void test_protein_join () {
	
	bioseq_protein test_seq = bioseq_protein_construct("TEFCLGFHRSEAWR");
	bioseq_protein test_seq2 = bioseq_protein_construct("RGTRCAAACTA");
	char res_string[] = "TEFCLGFHRSEAWRRGTRCAAACTA";
		
	bioseq_protein res_seq = bioseq_protein_join(test_seq, test_seq2);
	
	ok(strcmp(res_seq.sequence, res_string) == 0, "protein_join returns appropriate sequence");
	
	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&test_seq2);
	bioseq_protein_destruct(&res_seq);


	// Case 2 - both NULL
	test_seq = bioseq_protein_construct(NULL);
	test_seq2 = bioseq_protein_construct(NULL);
	char res_string2[] = "";
		
	res_seq = bioseq_protein_join(test_seq, test_seq2);
	
	ok(strcmp(res_seq.sequence, res_string2) == 0, "protein_join returns appropriate sequence");
	
	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&test_seq2);
	bioseq_protein_destruct(&res_seq);	


	// Case 2 - One NULL
	test_seq = bioseq_protein_construct(NULL);
	test_seq2 = bioseq_protein_construct("TEFCLGHR");
	char res_string3[] = "TEFCLGHR";
		
	res_seq = bioseq_protein_join(test_seq, test_seq2);
	
	ok(strcmp(res_seq.sequence, res_string3) == 0, "protein_join returns appropriate sequence");
	
	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&test_seq2);
	bioseq_protein_destruct(&res_seq);	
}


void test_dna_split () {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna res_seq1;
	bioseq_dna res_seq2;

	// Case 1 -- Even
	int index = 2;
	char res_str1[] = "AT";
	char res_str2[] = "GACAGCAGCAGCGAGCGACTTCGATA";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str1, res_seq1.sequence) == 0, "%s == %s", res_str1, res_seq1.sequence);
	ok(strcmp(res_str2, res_seq2.sequence) == 0, "%s == %s", res_str2, res_seq2.sequence);
	
	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	
	// Case 2 -- Odd
	index = 5;
	char res_str3[] = "ATGAC";
	char res_str4[] = "AGCAGCAGCGAGCGACTTCGATA";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str3, res_seq1.sequence) == 0, "%s == %s", res_str3, res_seq1.sequence);
	ok(strcmp(res_str4, res_seq2.sequence) == 0, "%s == %s", res_str4, res_seq2.sequence);

	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	
	// Case 3 -- Zero
	index = 0;
	char res_str5[] = EMPTY_STRING;
	char res_str6[] = TEST_SEQ;
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str5, res_seq1.sequence) == 0, "%s == %s", res_str5, res_seq1.sequence);
	ok(strcmp(res_str6, res_seq2.sequence) == 0, "%s == %s", res_str6, res_seq2.sequence);
	
	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	
	// Case 4 -- Overflow
	index = 41;
	char res_str7[] = TEST_SEQ;
	char res_str8[] = EMPTY_STRING;
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str7, res_seq1.sequence) == 0, "%s == %s", res_str7, res_seq1.sequence);
	ok(strcmp(res_str8, res_seq2.sequence) == 0, "%s == %s", res_str8, res_seq2.sequence);

	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);	

	// Case 5 -- Negative
	index = -3;
	char res_str9[] = EMPTY_STRING;
	char res_str10[] = TEST_SEQ;
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str9, res_seq1.sequence) == 0, "%s == %s", res_str9, res_seq1.sequence);
	ok(strcmp(res_str10, res_seq2.sequence) == 0, "%s == %s", res_str10, res_seq2.sequence);	

	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
}

void test_dna_substitution () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGATAGCAGCAGCGAGCGACTTCGATA";
		
	bioseq_dna res_seq = bioseq_dna_substitution(test_seq, 4, 'T');
	
	ok(strcmp(res_seq.sequence, res_string) == 0, "dna_substitution returns appropriate sequence");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_dna_deletion () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGAAGCAGCAGCGAGCGACTTCGATA";
		
	bioseq_dna res_seq = bioseq_dna_deletion(test_seq, 4);
	
	ok(strcmp(res_seq.sequence, res_string) == 0, "dna_delete returns appropriate sequence");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_dna_insertion () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGATCAGCAGCAGCGAGCGACTTCGATA";
		
	bioseq_dna res_seq = bioseq_dna_insertion(test_seq, 4, 'T');
	
	if (!ok(strcmp(res_seq.sequence, res_string) == 0, "dna_insertion returns appropriate sequence")) diag("Expected: %s, actual: %s", res_string, res_seq.sequence);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_dna_insertion_block () {

    bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
    char res_string[] = "ATGACCATAGCAGCAGCGAGCGACTTCGATA";

    bioseq_dna res_seq = bioseq_dna_insertion_block(test_seq, 5, "CAT");
    
    if (!ok(strcmp(res_seq.sequence, res_string) == 0, "dna_insertion_block returns appropriate sequence")) diag("Expected:; %s, actual: %s", res_string, res_seq.sequence);

    bioseq_dna_destruct(&test_seq);
    bioseq_dna_destruct(&res_seq);
}

void test_protein_split () {
	
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	bioseq_protein res_seq1;
	bioseq_protein res_seq2;


	// Case 1 -- Even
	int index = 2;
	char res_str1[] = "MT";
	char res_str2[] = "AAASDFD";
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str1, res_seq1.sequence) == 0, "%s == %s", res_str1, res_seq1.sequence);
	ok(strcmp(res_str2, res_seq2.sequence) == 0, "%s == %s", res_str2, res_seq2.sequence);
	
    bioseq_protein_destruct(&res_seq1);
    bioseq_protein_destruct(&res_seq2);

    // Case 2 -- Odd
	index = 5;
	char res_str3[] = "MTAAA";
	char res_str4[] = "SDFD";
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str3, res_seq1.sequence) == 0, "%s == %s", res_str3, res_seq1.sequence);
	ok(strcmp(res_str4, res_seq2.sequence) == 0, "%s == %s", res_str4, res_seq2.sequence);

    bioseq_protein_destruct(&res_seq1);
    bioseq_protein_destruct(&res_seq2);

	// Case 3 -- Zero
	index = 0;
	char res_str5[] = EMPTY_STRING;
	char res_str6[] = TEST_SEQ_PROT;
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str5, res_seq1.sequence) == 0, "%s == %s", res_str5, res_seq1.sequence);
	ok(strcmp(res_str6, res_seq2.sequence) == 0, "%s == %s", res_str6, res_seq2.sequence);

    bioseq_protein_destruct(&res_seq1);
    bioseq_protein_destruct(&res_seq2);

	// Case 4 -- Overflow
	index = 9;
	char res_str7[] = TEST_SEQ_PROT;
	char res_str8[] = EMPTY_STRING;
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str7, res_seq1.sequence) == 0, "%s == %s", res_str7, res_seq1.sequence);
	ok(strcmp(res_str8, res_seq2.sequence) == 0, "%s == %s", res_str8, res_seq2.sequence);
    
    bioseq_protein_destruct(&res_seq1);
    bioseq_protein_destruct(&res_seq2);

	// Case 5 -- Negative
	index = -3;
	char res_str9[] = EMPTY_STRING;
	char res_str10[] = TEST_SEQ_PROT;
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	ok(strcmp(res_str9, res_seq1.sequence) == 0, "%s == %s", res_str9, res_seq1.sequence);
	ok(strcmp(res_str10, res_seq2.sequence) == 0, "%s == %s", res_str10, res_seq2.sequence);	

	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&res_seq1);
	bioseq_protein_destruct(&res_seq2);
}

void test_frame_construct() {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_frame test_frame = bioseq_frame_construct(test_seq);
	
	int output = 0;
	
	output = output || strcmp(test_frame.frames[0].sequence,TEST_SEQ_PROT);
	output = output || strcmp(test_frame.frames[1].sequence,TEST_SEQ_PROT_2);
	output = output || strcmp(test_frame.frames[2].sequence,TEST_SEQ_PROT_3);
	output = output || strcmp(test_frame.frames[3].sequence,TEST_SEQ_PROT_4);
	output = output || strcmp(test_frame.frames[4].sequence,TEST_SEQ_PROT_5);
	output = output || strcmp(test_frame.frames[5].sequence,TEST_SEQ_PROT_6);
	
	ok(output == 0, "frame_construct: Returning all appropriate sequences.");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_frame_destruct(&test_frame);	
}

void test_frame_destruct() {
	bioseq_dna seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_frame frame = bioseq_frame_construct(seq);
	bioseq_dna_destruct(&seq);
	bioseq_frame_destruct(&frame);
	
	int res = 1;
	for (int i = 0; i < 6; i++) {
		res = res && frame.frames[i].sequence == NULL && frame.frames[i].charge == NULL;
	}
	
	ok(res == 1, "frame_destruct NULLs pointers.");
}

void test_frame_getopen() {
	bioseq_dna seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_frame frame = bioseq_frame_construct(seq);
	
	ok1(bioseq_frame_getopen(frame) == 6);
	
	bioseq_dna_destruct(&seq);
	bioseq_frame_destruct(&frame);
}

void test_string_similarity() {
	
	
	// Case 1 - no frameshift
	char test_s1[] = "ABCDEFG";
	char test_s2[] = "ACBDEFG";
	
	ok1(bioseq_string_similarity(test_s1, test_s2) == 2); 
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
	test_dna_join();
	test_dna_substitution();
	test_dna_insertion();
	test_dna_deletion();
    test_dna_insertion_block();
    test_protein_split();
	test_protein_join();
	test_frame_construct();
	test_frame_destruct();
	test_frame_getopen();
	test_string_similarity();
	
}

int main () {
    
    plan_no_plan();
    // plan_tests(14);
    
    run_tests();	
	
	
    return exit_status();
}
