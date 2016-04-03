#ifndef _LIST_H
#define _LIST_H

struct node {
    struct node* next;
    int data;
};

struct list {
    struct node* first;
    struct node* last;
};

#endif
