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

