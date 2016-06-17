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

void run_tests (void);

void test_dna_construct () {
	int test_int = 0;

	// Case 1 - normal use
	bioseq_dna test_seq = bioseq_dna_construct(E_COLI);
	
	test_int |= !(strcmp(test_seq.sequence, E_COLI) == 0);
	test_int |= !(test_seq.length == strlen(E_COLI));
		
	bioseq_dna_destruct(&test_seq);


	// Case 2 - empty string
	test_seq = bioseq_dna_construct("");
	
	test_int |= !(strcmp(test_seq.sequence, "") == 0);
	test_int |= !(test_seq.length == 0);
		
	bioseq_dna_destruct(&test_seq);


	// Case 2 - NULL string
	test_seq = bioseq_dna_construct(NULL);
	
	test_int |= !(strcmp(test_seq.sequence, "") == 0);
	test_int |= !(test_seq.length == 0);
		
	bioseq_dna_destruct(&test_seq);

	ok(test_int == 0, "dna_construct returns appropriate values.");
} 

void test_dna_destruct () {
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna_destruct(&test_seq);
	
	ok(!test_seq.sequence, "dna_destruct returns NULL.");
		
}

void test_protein_construct () {

	int test_int = 0;

	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	
	test_int |= !(strcmp(test_seq.sequence, TEST_SEQ_PROT) == 0);
	test_int |= !(test_seq.length == strlen(TEST_SEQ_PROT));
	
	bioseq_protein_destruct(&test_seq);

	// Case 2 - empty string
	test_seq = bioseq_protein_construct("");
	
	test_int |= !(strcmp(test_seq.sequence, "") == 0);
	test_int |= !(test_seq.length == 0);
		
	bioseq_protein_destruct(&test_seq);


	// Case 2 - NULL string
	test_seq = bioseq_protein_construct(NULL);
	
	test_int |= !(strcmp(test_seq.sequence, "") == 0);
	test_int |= !(test_seq.length == 0);
		
	bioseq_protein_destruct(&test_seq);
	
	ok(test_int == 0, "protein_construct returns expected values.");
} 

void test_protein_destruct () {
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	bioseq_protein_destruct(&test_seq);
	
	ok(!test_seq.sequence && !test_seq.charge, "Protein destructor pointers NULL.");			
}

void test_dna_sanitize () {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ_SANITIZE);
	ok(strcmp(test_seq.sequence, TEST_SEQ) == 0, "dna_sanitize correctly cleaning.");
	
	bioseq_dna_destruct(&test_seq);			
}

void test_protein_sanitize () {
	
	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT_SANITIZE);
	ok(strcmp(test_seq.sequence, TEST_SEQ_PROT) == 0, "protein_sanitize correctly cleaning.");
	
	bioseq_protein_destruct(&test_seq);			
}

void test_dna_complement () {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna test_comp = bioseq_dna_complement(test_seq);
	
	ok(strcmp(test_comp.sequence, TEST_SEQ_COMP) == 0, "dna_complement returns correct value");
	
	bioseq_dna_destruct(&test_comp);
	bioseq_dna_destruct(&test_seq);	
}

void test_dna_reverse () {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna test_rev = bioseq_dna_reverse(test_seq);
	
	ok(strcmp(test_rev.sequence, TEST_SEQ_REV) == 0, "dna_reverse returns correct value");
	
	bioseq_dna_destruct(&test_rev);
	bioseq_dna_destruct(&test_seq);
	
}

void test_dna_protein () {
	
	int test_int = 0;

	// Case 1 -- Start codon
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);	
	bioseq_protein test_prot = bioseq_dna_protein(test_seq, 0);	
	
	test_int |= !(strcmp(test_prot.sequence, TEST_SEQ_PROT) == 0);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
		
	// Case 2 -- Start and Stop
	test_seq = bioseq_dna_construct("AUGGCGAGGGAGUUAUGGUGA");
	test_prot = bioseq_dna_protein(test_seq, 0);
	
	test_int |= !(strcmp(test_prot.sequence, "MARELWX") == 0);

	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
	
	// Case 3 -- Misaligned , uneven
	test_seq = bioseq_dna_construct("AUGGCGAGGGAGUUAUGGUGA");
	test_prot = bioseq_dna_protein(test_seq, 1);
	
	test_int |= !(strcmp(test_prot.sequence, "WRGSYG") == 0);
		
	bioseq_dna_destruct(&test_seq);
	bioseq_protein_destruct(&test_prot);
	
	ok(test_int == 0, "dna_protein returns expected values.");
}

void test_dna_join () {
	
	int test_int = 0;

	// Case 1 - normal values
	bioseq_dna test_seq = bioseq_dna_construct("ATGCTA");
	bioseq_dna test_seq2 = bioseq_dna_construct("GCATA");
	char res_string[] = "ATGCTAGCATA";
		
	bioseq_dna res_seq = bioseq_dna_join(test_seq, test_seq2);
	
	test_int |= !(strcmp(res_seq.sequence, res_string) == 0);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&test_seq2);
	bioseq_dna_destruct(&res_seq);

	// Case 2 - null values

	test_seq = bioseq_dna_construct(NULL);
	test_seq2 = bioseq_dna_construct(NULL);
	char res_string2[] = "";
		
	res_seq = bioseq_dna_join(test_seq, test_seq2);
	
	test_int |= (strcmp(res_seq.sequence, res_string2) == 0);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&test_seq2);
	bioseq_dna_destruct(&res_seq);

	// Case 3 - single null value

	test_seq = bioseq_dna_construct(NULL);
	test_seq2 = bioseq_dna_construct("AGATAG");
	char res_string3[] = "AGATAG";
		
	res_seq = bioseq_dna_join(test_seq, test_seq2);
	
	test_int |= (strcmp(res_seq.sequence, res_string3) == 0);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&test_seq2);
	bioseq_dna_destruct(&res_seq);

	ok(test_int != 0, "dna_join returns expected values."); 
}

void test_protein_join () {
	
	int test_int = 0;

	bioseq_protein test_seq = bioseq_protein_construct("TEFCLGFHRSEAWR");
	bioseq_protein test_seq2 = bioseq_protein_construct("RGTRCAAACTA");
	char res_string[] = "TEFCLGFHRSEAWRRGTRCAAACTA";
		
	bioseq_protein res_seq = bioseq_protein_join(test_seq, test_seq2);
	
	test_int |= !(strcmp(res_seq.sequence, res_string) == 0);
	
	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&test_seq2);
	bioseq_protein_destruct(&res_seq);


	// Case 2 - both NULL
	test_seq = bioseq_protein_construct(NULL);
	test_seq2 = bioseq_protein_construct(NULL);
	char res_string2[] = "";
		
	res_seq = bioseq_protein_join(test_seq, test_seq2);
	
	test_int |= !(strcmp(res_seq.sequence, res_string2) == 0);
	
	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&test_seq2);
	bioseq_protein_destruct(&res_seq);	


	// Case 2 - One NULL
	test_seq = bioseq_protein_construct(NULL);
	test_seq2 = bioseq_protein_construct("TEFCLGHR");
	char res_string3[] = "TEFCLGHR";
		
	res_seq = bioseq_protein_join(test_seq, test_seq2);
	
	test_int |= !(strcmp(res_seq.sequence, res_string3) == 0);
	
	bioseq_protein_destruct(&test_seq);
	bioseq_protein_destruct(&test_seq2);
	bioseq_protein_destruct(&res_seq);

	ok(test_int == 0, "protein_join returns expected values.");
}


void test_dna_split () {
	
	int test_int = 0;

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_dna res_seq1;
	bioseq_dna res_seq2;

	// Case 1 -- Even
	int index = 2;
	char res_str1[] = "AT";
	char res_str2[] = "GACAGCAGCAGCGAGCGACTTCGATA";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str1, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str2, res_seq2.sequence) == 0);
	
	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	
	// Case 2 -- Odd
	index = 5;
	char res_str3[] = "ATGAC";
	char res_str4[] = "AGCAGCAGCGAGCGACTTCGATA";
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str3, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str4, res_seq2.sequence) == 0);

	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	
	// Case 3 -- Zero
	index = 0;
	char res_str5[] = EMPTY_STRING;
	char res_str6[] = TEST_SEQ;
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str5, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str6, res_seq2.sequence) == 0);
	
	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	
	// Case 4 -- Overflow
	index = 41;
	char res_str7[] = TEST_SEQ;
	char res_str8[] = EMPTY_STRING;
	
	bioseq_dna_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str7, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str8, res_seq2.sequence) == 0);

	bioseq_dna_destruct(&res_seq1);
	bioseq_dna_destruct(&res_seq2);
	bioseq_dna_destruct(&test_seq);

	ok(test_int == 0, "dna_split returns expected values.");
}

void test_dna_substitution () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGATAGCAGCAGCGAGCGACTTCGATA";
		
	bioseq_dna res_seq = bioseq_dna_substitution(test_seq, 4, 'T');
	
	ok(strcmp(res_seq.sequence, res_string) == 0, "dna_substitution returns expected values.");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_dna_deletion () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGAAGCAGCAGCGAGCGACTTCGATA";
		
	bioseq_dna res_seq = bioseq_dna_deletion(test_seq, 4);
	
	ok(strcmp(res_seq.sequence, res_string) == 0, "dna_delete returns expected values.");
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_dna_insertion () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGATCAGCAGCAGCGAGCGACTTCGATA";
		
	bioseq_dna res_seq = bioseq_dna_insertion(test_seq, 4, 'T');
	
	if (!ok(strcmp(res_seq.sequence, res_string) == 0, "dna_insertion returns expected values.")) diag("Expected: %s, actual: %s", res_string, res_seq.sequence);
	
	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_dna_insertion_block () {

	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	char res_string[] = "ATGACCATAGCAGCAGCGAGCGACTTCGATA";

	bioseq_dna res_seq = bioseq_dna_insertion_block(test_seq, 5, "CAT");
	
	if (!ok(strcmp(res_seq.sequence, res_string) == 0, "dna_insertion_block returns expected values.")) diag("Expected:; %s, actual: %s", res_string, res_seq.sequence);

	bioseq_dna_destruct(&test_seq);
	bioseq_dna_destruct(&res_seq);
}

void test_protein_split () {
	
	int test_int = 0;

	bioseq_protein test_seq = bioseq_protein_construct(TEST_SEQ_PROT);
	bioseq_protein res_seq1;
	bioseq_protein res_seq2;


	// Case 1 -- Even
	int index = 2;
	char res_str1[] = "MT";
	char res_str2[] = "AAASDFD";
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str1, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str2, res_seq2.sequence) == 0);
	
	bioseq_protein_destruct(&res_seq1);
	bioseq_protein_destruct(&res_seq2);

	// Case 2 -- Odd
	index = 5;
	char res_str3[] = "MTAAA";
	char res_str4[] = "SDFD";
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str3, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str4, res_seq2.sequence) == 0);

	bioseq_protein_destruct(&res_seq1);
	bioseq_protein_destruct(&res_seq2);

	// Case 3 -- Zero
	index = 0;
	char res_str5[] = EMPTY_STRING;
	char res_str6[] = TEST_SEQ_PROT;
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str5, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str6, res_seq2.sequence) == 0);

	bioseq_protein_destruct(&res_seq1);
	bioseq_protein_destruct(&res_seq2);

	// Case 4 -- Overflow
	index = 9;
	char res_str7[] = TEST_SEQ_PROT;
	char res_str8[] = EMPTY_STRING;
	
	bioseq_protein_split(test_seq, index, &res_seq1, &res_seq2);
	
	test_int |= !(strcmp(res_str7, res_seq1.sequence) == 0);
	test_int |= !(strcmp(res_str8, res_seq2.sequence) == 0);

	bioseq_protein_destruct(&res_seq1);
	bioseq_protein_destruct(&res_seq2);
	bioseq_protein_destruct(&test_seq);

	ok(test_int == 0, "protein_split returns expected values.");
}

void test_frame_construct() {
	
	bioseq_dna test_seq = bioseq_dna_construct(TEST_SEQ);
	bioseq_frame test_frame = bioseq_frame_construct(test_seq);
	
	int output = 0;
	
	output |= strcmp(test_frame.frames[0].sequence,TEST_SEQ_PROT);
	output |= strcmp(test_frame.frames[1].sequence,TEST_SEQ_PROT_2);
	output |= strcmp(test_frame.frames[2].sequence,TEST_SEQ_PROT_3);
	output |= strcmp(test_frame.frames[3].sequence,TEST_SEQ_PROT_4);
	output |= strcmp(test_frame.frames[4].sequence,TEST_SEQ_PROT_5);
	output |= strcmp(test_frame.frames[5].sequence,TEST_SEQ_PROT_6);
	
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

	char test_s1[] = "BASKET BALL FIRES";
	char test_s2[] = "MASTER FALL DIRES";
	
	ok(bioseq_string_similarity(test_s1, test_s2) == 12, "string_similarity returns expected value."); 
}

void test_string_insertions() {

	char test_s1[] = "ADGATDGATDGATGDGATTG";
	char test_s2[] = "ADAATDGATGGATGDGATTG";
	ok(bioseq_string_insertions(test_s1, test_s2) == 2, "string_insertions returns expected value."); 
}

void test_string_deletions() {

	char test_s1[] = "ADGATDGATDGATGDGATTG";
	char test_s2[] = "ADAATDGATGGATGDGATG";
	ok(bioseq_string_deletions(test_s1, test_s2) == 3, "string_deletions returns expected value."); 
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
	test_string_insertions();
	test_string_deletions();
}

int main () {
	
	plan_no_plan();
	// plan_tests(14);
	
	run_tests();
	
	
	return exit_status();
}
