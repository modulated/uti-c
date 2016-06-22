#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

struct node {
	struct node* next;
	void* data;
};

struct list {
	struct node* first;
	struct node* last;
};

struct hashmap {
	char* array[256];
	int num;
};

struct hashnode {
	void* name;
	char hash;
	void* data;
};

struct list list_create(void* data) {
	struct list rlist;
	struct node rnode;
	rlist.first = &rnode;
	rlist.last = &rnode;
	rlist.first->data = data;
	rlist.first->next = NULL;
	return rlist;
}

int list_append(struct list* list, void* data) {
	struct node new;
	new.data = data;
	new.next = NULL;

	list->last->next = &new;
	list->last = &new;

	return 0;

}

int list_prepend(struct list* list, void* data) {
	struct node new;
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

struct hashmap data_hashmap_construct() {
	struct hashmap out;
	out.num = 0;

	return out;
}

void data_hashmap_destruct(struct hashmap* hash) {
	
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

int main(int argc, char* argv[]) {
	
	for (int i = 1; i < argc; i++) {
		char res = data_hash_minihash(argv[i]);
		printf("%d\n", res);
	}
	return 0;
}
