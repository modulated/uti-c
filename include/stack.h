#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
	int size;
	int capacity;
	int* array;
} stack_int_t;

stack_int_t stack_new(int capacity);
void stack_del(stack_int_t* stack);
void stack_push(stack_int_t* stack, int value);
int stack_pop(stack_int_t* stack);
int stack_peek(stack_int_t* stack);

#endif
