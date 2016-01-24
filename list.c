#include <stdio.h>

struct Node {
    struct Node* next;
    int data;
} Node;

typedef struct {
    struct Node* first;
    struct Node* last;
} List; 

int main() {
    List mylist;
    struct Node mynode;
    mynode.data = 1;
    mynode.next = NULL;
    
    //printf("List val: %d", mylist->first)
}