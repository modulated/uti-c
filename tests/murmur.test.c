#include <tap.h>
#include <murmur.h>
#include <string.h>
#include <stdio.h>

void test_all(void);

static void hex32(uint32_t* hash, char* buf) {
	sprintf(buf, "%08x", *hash);
}

static void hex128(uint32_t hash[4], char *buf) {
	sprintf(buf, "%08x%08x%08x%08x", hash[0], hash[1], hash[2], hash[3]);
}

int main() {
	plan();
	test_all();
	
	return exit_status();
}

void test_murmur_hash_32 () 
{
	char key[] = "Hello, world!";
	char out[33];
	uint32_t hash[4];
	int len = strlen(key);
	int seed = 1234;
	char expect[] = "faf6cdb3";

	murmur_hash_32 (key, len, seed, hash);
	hex32(hash, out);
	ok(strcmp(out, expect) == 0, "Expected %s, got %s", expect, out);
}

void test_murmur_hash_128 () 
{
	char key[] = "Hello, world!";
	char out[33];
	uint32_t hash[4];
	int len = strlen(key);
	int seed = 123;
	char expect[] = "8743acad421c8c73d373c3f5f19732fd";

	murmur_hash_128 (key, len, seed, hash);
	hex128(hash, out);
	ok(strcmp(out, expect) == 0, "Expected %s, got %s", expect, out);
}

void test_all()
{
	test_murmur_hash_32();
	test_murmur_hash_128();
}
