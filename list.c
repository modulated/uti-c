#include <stdio.h>
#include "list.h"

struct node {
    struct node* next;
    int data;
};

struct list {
    struct node* first;
    struct node* last;
};

struct list list_create() {
	struct list rlist;
	struct node rnode;
	rlist.first = &rnode;
	rlist.last = &rnode;
	return rlist;
}

int list_append(struct list* list, int data) {
	struct node new;
	new.data = data;
	new.next = NULL;

	list->last->next = &new;
	list->last = &new;

	return 0;

}

int list_prepend(struct list* list, int data) {
	struct node new;
	new.data = data;
	new.next = list->first;

	list->first = &new;

	return 0;
}

struct node list_iterate(struct list* list) {
	return struct node;
}	

struct node list_find(struct list* list, int data) {
	struct node rnode;
	
	
	return rnode;
}
