#include "tap.h"
#include "json.h"
#include <stdlib.h>

void test_all(void);

int main() {
	
	plan();
	test_all();
	
	return exit_status();
}

void test_create_element(void) {
	union JSON_DATA integ;
	integ.num = 23;
	struct json_element test_el = json_create_element(JSON_NUMBER, integ, "a");
	ok(test_el.type == JSON_NUMBER, "create_element returns valid list.") || diag("Element does not have valid properties.");
}

void test_create_object(void) {
	
	struct json_object test_obj = json_create_obj(2,malloc(2));

	ok(test_obj.number == 2, "create_object returns valid number") || diag("Value not expected.");
}

void test_all(void) {
	
	test_create_element();
	test_create_object();

}
