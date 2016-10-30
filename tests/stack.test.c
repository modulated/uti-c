#include <tap.h>
#include "stack.h"
#include <stdio.h>

void test_all();
int main()
{
	plan();

	test_all();
	return exit_status();
}

void test_new()
{
	int capacity = 16;
	stack_int_t stack = stack_new(capacity);

	ok(
		stack.capacity == capacity &&
		stack.array != NULL,
		"stack_new."
	);
}

void test_del()
{
	int capacity = 16;
	stack_int_t stack = stack_new(capacity);

	stack_del(&stack);

	ok(
		stack.capacity == 0 &&
		stack.size == 0 &&		
		stack.array == NULL,
		"stack_del."
	);
}

void test_push()
{
	int capacity = 16;
	stack_int_t stack = stack_new(capacity);

	int a = 12;
	stack_push(&stack, a);

	ok(
		stack.array[0] == a &&
		stack.size == 1,
		"stack_push ok."
	);
}

void test_pop()
{
	int capacity = 16;
	stack_int_t stack = stack_new(capacity);

	int a = 12;
	stack_push(&stack, a);
	int res = stack_pop(&stack);

	ok(
		a == res &&
		stack.size == 0,
		"stack_pop."
	) ||
	diag("expected %d, got %d", a, res);
}

void test_peek()
{
	int capacity = 16;
	stack_int_t stack = stack_new(capacity);

	int a = 12;
	stack_push(&stack, a);
	int res = stack_peek(&stack);

	ok(
		a == res &&
		stack.size == 1,
		"stack_peek."
	) || diag("expected %d got %d", a, res);
}

void test_all()
{
	test_new();
	test_del();
	test_push();
	test_pop();
	test_peek();
}
