#ifndef _LIST_H
#define _LIST_H

struct list list_create(void* data);

int list_append(struct list* list, void* data);

int list_prepend(struct list* list, void* data);

struct node list_iterate(struct list* list);

struct node list_find(struct list* list, void* data);

#endif
