#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

struct data_list data_list_construct(int data) {
	struct data_list rlist;
	struct data_list_node rnode;
	rlist.first = &rnode;
	rlist.last = &rnode;
	rlist.first->data = data;
	rlist.first->next = NULL;
	return rlist;
}

int data_list_append(struct data_list* list, int data) {
	struct data_list_node new;
	new.data = data;
	new.next = NULL;

	list->last->next = &new;
	list->last = &new;

	return 0;

}

int data_list_prepend(struct data_list* list, int data) {
	struct data_list_node new;
	new.data = data;
	new.next = list->first;

	list->first = &new;

	return 0;
}

// struct node list_iterate(struct list* list) {
// 	return struct node;
// }	

// struct node list_find(struct list* list, void* data) {
// 	struct node rnode;
	
	
// 	return rnode;
// }

struct data_hashmap data_hashmap_construct() {
	struct data_hashmap out;
	out.num = 0;

	return out;
}

void data_hashmap_destruct(struct data_hashmap* hash) {
	
	free(hash->array);
}

unsigned char data_hash_minihash(char* in) {

	int length = strlen(in);
	unsigned char xor = (unsigned char)length;
	for (int i = 0; i < length; i++) {
		xor ^= (unsigned char)in[i];
		xor ^= xor<<(length%8) | xor>>(8-(length%8));
	}
	int rotator = (length*7^xor)%8;
	xor ^= xor<<rotator | xor>>(8-rotator);
	return xor;
}

