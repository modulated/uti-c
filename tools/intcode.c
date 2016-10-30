#include <syn.h>

// Dummy Program, cont array of 4 bytes 
int dummy[] = {
	SET, 0x8, // SET 8
	SET, 0x2, // SET 2
	SET, 0xF, // SET 15
	ADD,
	GET, // Return to output[0]
	GET,
	CLR, // Set output[0] to 0
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
	
	printf("Result: %d\n", output[0]);

	return 0;
}
