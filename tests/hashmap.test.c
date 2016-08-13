#include "tap.h"
#include <stdlib.h>
#include <stdio.h>
#include "../include/hashmap.h"


void test_hashmap_new (void)
{
	map_t map = hashmap_new();
	ok(map != NULL, "hashmap_new.");
	
	hashmap_free(map);
}

void test_hashmap_free (void)
{	
	map_t map = hashmap_new();
	hashmap_free(map);

	skip(map == NULL, "hashmap_free.") ||
	diag("Expected: %p got: %p", NULL, map);
}

void test_hashmap_add (void)
{
	map_t map = hashmap_new();
	const char key[] = "test";
	const int val = 24;
	int error = hashmap_add(map, (char*)key, (void*)&val);
	ok(error == HASHMAP_OK, "hashmap_add.") || diag("Expected: %d got: %d", HASHMAP_OK, error);

	hashmap_free(map);
}

void test_hashmap_get (void)
{
	map_t map = hashmap_new();
	const char key[] = "No sleep";
	const int val = 24;
	any_t set_box = (any_t)&val;
	int error = hashmap_add(map, (char*)key, set_box);
	if (error != HASHMAP_OK)
	{
		printf("Error adding to hashmap: %d\n", error);
		exit(error);
	}
	
	any_t ptr;
	error = hashmap_get(map, (char*)key, &ptr);
	int get_val = *(int*)ptr;
	
	if (error != HASHMAP_OK)
	{
		char error_str[32];
		hashmap_error(error, error_str, 32);
		printf("ERROR getting from hashmap: %s\n", error_str);
		exit(error);
	}

	ok(get_val == 24, "hashmap_get.") || diag("Expected: %d got: %d", val, get_val);

	hashmap_free(map);
}

void test_hashmap_del()
{
	map_t map = hashmap_new();
	const char key[] = "No sleep";
	const int val = 24;
	any_t set = (any_t)&val;
	int error = hashmap_add(map, (char*)key, set);
	if (error != HASHMAP_OK)
	{
		char error_str[32];
		hashmap_error(error, error_str, 32);
		printf("ERROR adding to hashmap: %s\n", error_str);
		exit(error);
	}
	
	any_t ptr;
	error = hashmap_get(map, (char*)key, &ptr);
	
	if (error != HASHMAP_OK)
	{
		char error_str[32];
		hashmap_error(error, error_str, 32);
		printf("ERROR getting from hashmap: %s\n", error_str);
		exit(error);
	}

	int del_error = hashmap_del(map, (char*)key);

	int get_error = hashmap_get(map, (char*)key, &ptr);

	ok(
		del_error == HASHMAP_OK &&
		get_error == HASHMAP_MISSING,
		"hashmap_del.") ||
	diag("Del: %d Get: %d\n", del_error, get_error);

	hashmap_free(map);
}

void test_hashmap_len()
{
	map_t map = hashmap_new();
	const int exp_init = 0;
	int len_init = hashmap_len(map);

	const char key[] = "key";	
	hashmap_add(map, (char*)key, (any_t)&key);
	const int exp_add = 1;
	int len_add = hashmap_len(map);

	hashmap_add(map, (char*)key, (any_t)&key);
	int len_readd = hashmap_len(map);

	hashmap_del(map, (char*)key);
	int len_del = hashmap_len(map);

	ok(
		len_init == exp_init &&
		len_add == exp_add &&
		len_readd == exp_add &&
		len_del == exp_init,
		"hashmap_len."
	);

	hashmap_free(map);
}

void test_all (void)
{
	test_hashmap_new();
	test_hashmap_free();
	test_hashmap_add();
	test_hashmap_get();
	test_hashmap_del();
	test_hashmap_len();
}

int main ()
{
	plan();
	test_all();
	return exit_status();
}
