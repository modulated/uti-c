#ifndef SYN_H
#define SYN_H

#include <stack.h>
#include <stdio.h>
#include <sys/stat.h>

#define DEBUG

#define STACK_SIZE 256
#define OUT_SIZE 4

stack_int_t stack;
int output[OUT_SIZE];

#define POP stack_pop(&stack)
#define PUSH(x) stack_push(&stack, x)

/* BASIC OPCODES */
#define SRT		0x53594E2E // Start of op codes
#define	END 	0xFFFFFFFF // End of op codes
#define	NOP 	0x00000000 // No operation
#define	SET 	0x00000002 // Push value onto stack
#define	GET 	0x00000003 // Return value from stack into output register
#define CLR 	0x00000004 // Clear top value from output register

/* CONTROL FLOW */
#define JMP 	0x0000000A // Jump to program buffer address
#define JMZ 	0x0000000B // Jump to program buffer address of next value if stack zero 

/* MATHS */
#define INC		0x00000100 // Increment first stack element
#define DEC		0x00000101 // Decrement first stack element
#define	ADD 	0x0000010A // Add first two stack elements
#define	SUB 	0x0000010B // Subtract second element from first
#define	MUL 	0x0000010C // Multiply first two stack elements
#define	DIV 	0x0000010D // Divide first element by second
#define	MOD 	0x0000010E // Modulo division

/* MEMORY */
#define LOD

/* STRINGS */
#define PST 	0x00000101 // Push string onto stack in reverse order
#define GST 	0x00000102 // Get nul terminated string off stack in order

enum BYTE_ERROR {
	ERR_UNKNOWN_ERROR,
	ERR_OUTPUT_OVERFLOW,
	ERR_ILLEGAL_INSTRUCTION,
	ERR_STACK_OVERFLOW,
};

int execute_program(int* input, int* output);
int* read_file(char* file);
void dump_program(int* prog);
void dump_output();
int endian_swap(int in);
void endian_swap_buffer(int* buffer);

#endif
