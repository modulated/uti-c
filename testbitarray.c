#include <stdio.h>
#include <string.h>
#include "bitarray.h"

int running = 1;
struct bitarray ba;
struct bitarray* bitarray = &ba;

void execute (char command[]);
void getsize (int* length);
int getposition ();

int main () {
    
    int length;
    char command[20];
    
    getsize(&length);
    
    ba = bitarray_create(length);
    
    while (running) {
        printf("Enter command: ");
        scanf("%s",command);
        execute(command);  
    }
    
    bitarray_destroy(&ba);
    return 0;
}

void getsize (int* length) {
    printf("How many bits? ");
    scanf("%d",length);
}

void execute (char command[]) {
    
    if (strcmp(command, "flip")==0){
        int position = getposition();
        bitarray_flip(bitarray, position);
        
    }
    
    else if (strcmp(command, "read")==0){
        int position = getposition();
        printf("Position %d: %d\n",position, bitarray_read(bitarray, position));
    }
    
    else if (strcmp(command, "on")==0) {
        printf("on\n");
    }
    
    else if (strcmp(command, "exit")==0) {
        running = 0;
    }
    
    else {
        printf("No command matched\n");
    }
}

int getposition () {
    int position;
    printf("What position? ");
    scanf("%d",&position);
    return position;
}