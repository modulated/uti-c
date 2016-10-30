#include <syn.h>

int op_get () {
	#ifdef DEBUG
		puts("GET");
	#endif

	return POP;
}

void op_set (int x) {
	#ifdef DEBUG
		puts("SET");
	#endif

	PUSH(x);
}

void op_clr () {
	#ifdef DEBUG
		puts("CLR");
	#endif


}

void op_add () {
	#ifdef DEBUG
		puts("ADD");
	#endif

	PUSH(POP + POP);
}

void op_sub() {
	PUSH(POP - POP);

	#ifdef DEBUG
		puts("SUB");
	#endif
}

void op_mul() {
	PUSH(POP * POP);
	
	#ifdef DEBUG
		puts("MUL");
	#endif
}

void op_div() {
	PUSH(POP / POP);
	
	#ifdef DEBUG
		puts("DIV");
	#endif
}

void op_mod() {
	PUSH(POP % POP);
	
	#ifdef DEBUG
		puts("MOD");
	#endif
}


int execute_program(int* input, int* output)
{
	int index = 0;
	int out_counter = 0;
	int cur = 0;

	while ((cur = input[index++]) != END)
	{
		switch(cur)
		{
			case GET: 
				if (out_counter < OUT_SIZE) 
				{
					output[out_counter++] = op_get();
					break;
				}
				else return ERR_OUTPUT_OVERFLOW;
				 
			case SET:
				op_set(input[index++]);
				break;

			case CLR:
				op_clr();
				out_counter--;
				output[out_counter] = 0;
				break;
				
			case JMP:
				index = input[index];
				printf("JMP to %d\n", index);
				break;

			case ADD:
				op_add();
				break;
			
			case SUB:
				op_sub();
				break;
			
			case MUL:
				op_mul();
				break;
			
			case MOD:
				op_mod();
				break;

			case NOP: break;
			default: return ERR_ILLEGAL_INSTRUCTION;
		}
	}

	return 0;
}

int* read_file(char* file)
{
	FILE* fp = fopen(file, "rb");
	struct stat info;
	stat(file, &info);

	int head = 0;
	fread(&head, sizeof(int), 1, fp);
	head = endian_swap(head);
	if (head != SRT)
	{
		puts("File not byte code...");
		printf("Expecting %x \t Got %x\n", SRT, head);
		exit(1);
	}

	int* buffer = malloc(info.st_size);
	fread(buffer, info.st_size, sizeof(int), fp);
	fclose(fp);
	
	return buffer;
}

void endian_swap_buffer (int* buffer)
{
	int i = 0;
	while (buffer[i] != END)
	{
		buffer[i] = endian_swap(buffer[i]);
		i++;
	}
}

void dump_program(int* prog)
{
	int cur = 0;
	while (prog[cur] != END)
	{
		printf("%i: %X\n", cur, prog[cur]);
		cur++;
	}
	printf("%i: %X\n", cur, prog[cur]);
}

void dump_output()
{
	for (int i = 0; i < OUT_SIZE; i++)
	{
		printf("%d: %d\n", i, output[i]);
	}
}

int endian_swap (int in)
{
	return 
	((in >> 24) & 0x000000ff) | // move byte 3 to byte 0
	((in <<  8) & 0x00ff0000) | // move byte 1 to byte 2
	((in >>  8) & 0x0000ff00) | // move byte 2 to byte 1
	((in << 24) & 0xff000000); // byte 0 to byte 3
}
