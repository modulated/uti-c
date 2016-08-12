#ifndef _LIST_H
#define _LIST_H

struct data_list_node {
	struct data_list_node* next;
	int data;
};

struct data_list {
	struct data_list_node* first;
	struct data_list_node* last;
};

struct data_list data_list_construct (int data);

int data_list_append(struct data_list* list, int data);

int data_list_prepend(struct data_list* list, int data);

int data_list_find(struct data_list* list, int data);


#endif
