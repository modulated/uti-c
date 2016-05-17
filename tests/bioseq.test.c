#include <stdio.h>
#include <string.h>
#include "test.h"
#include "../src/bioseq.c"
#include "../src/color.h"

#define TEST_SEQ "ATATGACAGCAGCAGCGAGCGACTTCGA"

int test_count = 0;

static char* test_constructor_destructor () {
	bioseq test_seq = bioseq_new(SEQUENCE_DNA, TEST_SEQ);
	
	test_assert(KRED"ERROR: bioseq_new does not return appropriate struct"RESET, strcmp(test_seq.sequence, TEST_SEQ) == 0);
	test_assert(KRED"ERROR: bioseq_new does not return appropriate sequence_type"RESET, test_seq.type == SEQUENCE_DNA);
	puts(KGRN"bioseq_new PASSED"RESET);
	
	bioseq_delete(test_seq);
	test_assert(KRED"ERROR: bioseq_delete fails"RESET, test_seq.sequence == NULL);
	puts(KGRN"bioseq_delete PASSED"RESET);
	
	return 0;
} 

static char* test_complement () {
	bioseq test_seq = bioseq_new(SEQUENCE_DNA, TEST_SEQ);
	
	test_assert(KRED"ERROR: bioseq_new does not return appropriate struct"RESET, strcmp(test_seq.sequence, TEST_SEQ) == 0);
	test_assert(KRED"ERROR: bioseq_new does not return appropriate sequence_type"RESET, test_seq.type == SEQUENCE_DNA);
	
	bioseq_delete(test_seq);
	
	puts(KGRN"test_constructor_destructor PASSED"RESET);
	return 0;
} 

static char* test_all() {
	
	test_run_test(test_constructor_destructor);
	
	return 0;
}

int main() {
	
	char* result = test_all();
	
	if (result !=0) {
		printf("%s\n", result);
	}
	
	else {
		puts(KGRN"ALL TEST PASSED"RESET);
	}
	
	printf("Tests run: "KBLU"%d\n"RESET, test_count);
	
	return result != 0;
}
