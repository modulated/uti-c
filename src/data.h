#ifndef _LIST_H
#define _LIST_H

struct data_list list_create(void* data);

int list_append(struct data_list* list, void* data);

int list_prepend(struct data_list* list, void* data);

struct data_node list_iterate(struct data_list* list);

struct data_node list_find(struct data_list* list, void* data);


#endif
