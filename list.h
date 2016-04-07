#ifndef _LIST_H
#define _LIST_H

struct list list_create(int data);

int list_append(struct list* list, int data);

int list_prepend(struct list* list, int data);

struct node list_iterate(struct list* list);

struct node list_find(struct list* list, int data);

#endif
