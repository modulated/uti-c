#include <syn.h>
// #define DEBUG

// Dummy Program, cont array of 4 bytes 
int dummy[] = {
	SET, 0x8, // SET 8
	SET, 0x2, // SET 2
	SET, 0xF, // SET 15
	ADD,
	OUT,
	INI,
	ADD,
	INI,
	OUT,
	OUT,
	END,
};


int main(int argc, char* argv[])
{	
	stack = stack_new(STACK_SIZE);
	int* program = NULL;

	if (argc == 2)
	{		
		program = read_file(argv[1]);
		endian_swap_buffer(program);
		execute_program(program, output);
	}
	else if (argc == 3)
	{
		program = dummy;
		execute_program(dummy, output);
	}
	else
	{		
		puts("Please supply one file as argument.");
		return 1;
	}

	#ifdef DEBUG
		dump_program(program);
		dump_output();
	#endif
	
	printf("Output 0: %d\n", output[0]);

	return 0;
}
