#include "stack.h"

stack_int_t stack_new(int capacity)
{
	stack_int_t s;
	s.capacity = capacity;
	s.size = 0;
	s.array = malloc(capacity * sizeof(int));

	return s;
}

void stack_del(stack_int_t* stack)
{
	stack->capacity = 0;
	stack->size = 0;
	free(stack->array);
	stack->array = NULL;
}

void stack_push(stack_int_t* stack, int value)
{
	stack->array[stack->size] = value;

	stack->size++;
}

int stack_pop(stack_int_t* stack)
{
	if (stack->size == 0) return -1;

	stack->size--;
	return stack->array[stack->size];
}

int stack_peek(stack_int_t* stack)
{
	if (stack->size == 0) return -1;

	return stack->array[stack->size - 1];
}
