#include <stdio.h>
#include <string.h>
#include "bitarray.h"
#include "color.h"

int running = 1;
struct bitarray ba;
struct bitarray* bitarray = &ba;

void execute (char command[]);
void getsize (int* length);
int getposition ();

int main () {
    
    int length;
    char command[20];
    printf(KWHT"Welcome to the bitarray test suite...\n"RESET);
    getsize(&length);
    
    ba = bitarray_create(length);
    
    while (running) {
        printf(KRED"Enter command: "RESET);
        scanf("%s",command);
        execute(command);  
    }
    
    bitarray_destroy(&ba);
    return 0;
}

void getsize (int* length) {
    printf(KBLU"How many bits? "RESET);
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
    	bitarray_on(bitarray, getposition());
	}
    
    else if (strcmp(command, "off")==0) {
        bitarray_off(bitarray, getposition());
    }
    
    else if (strcmp(command, "resize")==0) {
        printf("Enter new size: ");
		int size;
		scanf("%d",&size);
		bitarray_resize(bitarray,size);
    }
    
    else if (strcmp(command, "exit")==0 || strcmp(command, "quit")==0 || strcmp(command, "q")==0) {
        printf(KBLU"Bye..."RESET);
        running = 0;
    }
    
    else {
        printf("No command matched\n");
    }
}

int getposition () {
    int position;
    printf(KYEL"What position? "RESET);
    scanf("%d",&position);
    return position;
}
