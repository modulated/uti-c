#include <stdio.h>
#include "list.h"

struct node {
    struct node* next;
    void* data;
};

struct list {
    struct node* first;
    struct node* last;
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
