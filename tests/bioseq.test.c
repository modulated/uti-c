#include "test.h"
#include "../src/bioseq.c"
#include "../src/color.h"

#define TEST_SEQ 		"ATATGACAGCAGCAGCGAGCGACTTCGA"
#define TEST_SEQ_COMP 	"TATACTGTCGTCGTCGCTCGCTGAAGCT"
#define TEST_SEQ_REV 	"AGCTTCAGCGAGCGACGACGACAGTATA"

int test_count = 0;



static char* test_constructor () {
	bioseq test_seq = bioseq_new(SEQUENCE_DNA, TEST_SEQ);
	
	test_assert(KRED"ERROR: bioseq_new does not return appropriate struct"RESET, strcmp(test_seq.sequence, TEST_SEQ) == 0);
	test_assert(KRED"ERROR: bioseq_new does not return appropriate sequence_type"RESET, test_seq.type == SEQUENCE_DNA);
	puts("bioseq_new() \t\t "KGRN"PASSED"RESET);
	
	
	return 0;
} 

static char* test_destructor () {
	bioseq test_seq = bioseq_new(SEQUENCE_DNA, TEST_SEQ);
	bioseq_delete(&test_seq);
	
	test_assert(KRED"ERROR: bioseq_delete does not NULL pointers"RESET, !test_seq.sequence && !test_seq.charge);
	
	puts("bioseq_delete() \t "KGRN"PASSED"RESET);	
	return 0;
}

static char* test_complement () {
	
	bioseq test_comp = bioseq_complement(bioseq_new(SEQUENCE_DNA, TEST_SEQ));
	
	test_assert(KRED"ERROR: bioseq_complement does not return expected value"RESET, strcmp(test_comp.sequence, TEST_SEQ_COMP) == 0);
	
	bioseq_delete(&test_comp);
	
	puts("bioseq_complement() \t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_reverse () {
	
	bioseq test_rev = bioseq_reverse(bioseq_new(SEQUENCE_DNA, TEST_SEQ));
	
	test_assert(KRED"ERROR: bioseq_reverse does not return expected value"RESET, strcmp(test_rev.sequence, TEST_SEQ_REV) == 0);
	
	bioseq_delete(&test_rev);
	
	puts("bioseq_reverse() \t "KGRN"PASSED"RESET);
	return 0;
}

static char* test_all() {
	
	test_run(test_constructor);
	test_run(test_destructor);
	test_run(test_complement);
	test_run(test_reverse);
	
	
	return 0;
}

int main() {
	
	char* result = test_all();
	
	if (result !=0) {
		printf("%s\n", result);
	}
	
	else {
		puts(KGRN"ALL TESTS PASSED"RESET);
	}
	
	printf("Tests run: "KBLU"%d\n"RESET, test_count);
	
	return result != 0;
}
