#include <tap.h>
#include <bloom.h>

void test_all(void);

int main() {
	plan();
	test_all();
	
	return exit_status();
}

void test_bloom_new()
{
	int size = 8;
	int hashes = 4;
	bloom_filter_t filter;
	filter = bloom_new(size, hashes);
	
	ok(
		filter.num_members == 0 &&
		filter.num_hashes == hashes &&
		filter.array_length == size,
		"bloom_new ok."
	);
}

void test_bloom_insert()
{
	int size = 8;
	int hashes = 4;
	char string[] = "test";
	bloom_filter_t filter;

	filter = bloom_new(size, hashes);
	bloom_insert(&filter, string, 4);
	ok(
		filter.num_members == 1,
		"bloom_insert ok."
	);
}

void test_bloom_query()
{
	int size = 8;
	int hashes = 4;
	char string[] = "test";
	bloom_filter_t filter;

	filter = bloom_new(size, hashes);
	bloom_insert(&filter, string, 4);
	int res = bloom_query(&filter, string, 4);

	ok(
		res == 1,
		"bloom_query ok."
	) |
	diag(
		"Expected %d got %d.",
		1,
		res
	);
}

void test_all()
{
	test_bloom_new();
	test_bloom_insert();
	test_bloom_query();
}
