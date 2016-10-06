#include <tap.h>
#include <ev.h>

void test_all();

int main()
{
	plan();
	test_all();
	return exit_status();
}

void test_router_new()
{
	router_t* r = router_new();
	ok(
		r->mqueue_size == 0 &&
		r->receiver_size == 0,
		"router_new."
	);

	router_free(r);
}

void test_router_free()
{
	router_t* r = router_new();
	router_free(&r);

	ok(
		r == NULL,
		"router_free."
	);
}

void test_all()
{
	test_router_new();
	test_router_free();
}
