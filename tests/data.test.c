#include "tap.h"
#include "../src/data.h"
#include <stdlib.h>

void test_all(void);

int main() {
	
	plan();
	test_all();
	
	return exit_status();
}

void test_list_construct(void) {
	
	struct data_list test_list = data_list_construct(1);
	ok(test_list.first != NULL, "list_construct returns valid list.") || diag("Stack does not have valid size.");
}

void test_list_append(void) {
	
	struct data_list test_list = data_list_construct(1);
	data_list_append(&test_list, 2);

	ok(test_list.last->data == 2, "list_append correctly appends to list.") || diag("Value not expected.");
}

void test_list_prepend(void) {
	
	struct data_list test_list = data_list_construct(1);
	data_list_prepend(&test_list, 2);

	ok(test_list.first->data == 2, "list_prepend correctly prepends to list.") || diag("Value not expected.");
}

void test_all(void) {
	
	test_list_construct();
	test_list_append();
	test_list_prepend();

}
