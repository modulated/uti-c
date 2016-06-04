#include <stdio.h>
#include <stdlib.h>
#include "bioseq.h"
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

// Version
#define VERSION "0.1.2"

// Flag defines
#define RNA_F 0x1<<6
#define DNA_F 0x1<<5
#define PROTEIN_F 0x1<<4
#define BUFFER_F 0x1<<3
#define OUTFILE_F 0x1<<2
#define STDINPUT_F 0x1

// Global vars
static int flags;
static FILE* infilepointer = NULL;
static FILE* outfilepointer = NULL;
static char* infilestring = NULL;
static char* outfilestring = NULL;
static unsigned buffer_size = 1000; 


void print_help(void) {
    puts(        
        "Usage: seqc [-hiv] [-b buffer size (default 1000)] [-o output] [input file]\n"
		"Arguments:\n"
		"-h \tShow this message\n"
		"-v \tPrint version info.\n"
	);
}

void print_version(void) {
    puts(
        "seqc bioinformatics tool "VERSION"\n"
		"written by modulated"
     );
}

/*
* File Handling Functions
*/

void open_infile() {
    infilepointer = fopen(infilestring, "r");	
	
	if (infilepointer == NULL) {
		puts("Error: cannot access input file.");
		printf("%s\n",strerror(errno));
		exit(errno);
	}
}

void open_outfile() {
    outfilepointer = fopen(outfilestring, "w");
	
	if (outfilepointer == NULL) {
		puts("Error: cannot access output file.");
		printf("%s\n",strerror(errno));
		exit(errno);
	}
}

void close_infile() {	
	fclose(infilepointer);
}

void close_outfile() {	
	fclose(outfilepointer);
}

char read_stdin() {
    char in = getc(stdin);
    if (in == EOF) {
        return '\0';
    }
	
    if (NUCLEOTIDE_SANITIZE(toupper(in))) return in;
	else {		
		return read_stdin();
	}		
}

char read_infile() {
    char in = fgetc(infilepointer);
    if (in == EOF) {
        return '\0';
    }
	
	if (NUCLEOTIDE_SANITIZE(toupper(in))) return in;
	else {
		return read_infile();
	}
}

void write_outfile(char out) {
	if (putc(out, outfilepointer) == EOF)
	{
		puts("Error: cannot write to file.");
		exit(1);
	}
}

/*
** Loop Functions
*/

void stdin_stdout_loop() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	
	while (cont == 1) {
		in[0] = read_stdin();
		in[1] = read_stdin();
		in[2] = read_stdin();		
		
		char out = bioseq_codon_protein(in[0], in[1], in[2]);
		
		if(putchar(out) == EOF) {
			puts("Error: cannot write to stdout.");			
			exit(1);
		}
		if (out == '?' || out == '\0') cont = 0;
	}
	putchar('\n');
}

void filein_stdout_loop() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	open_infile();
	
	while (cont == 1) {
		in[0] = read_infile();
		in[1] = read_infile();
		in[2] = read_infile();		
		
		char out = bioseq_codon_protein(in[0], in[1], in[2]);
		
		fputc(out, stdout);
		if (out == '?' || out == '\0') cont = 0;
	}
	puts("");
	
	close_infile();
}

void filein_fileout_loop() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	open_infile();
	open_outfile();
	
	while (cont == 1) {
		in[0] = read_infile();
		in[1] = read_infile();
		in[2] = read_infile();		
		
		char out = bioseq_codon_protein(in[0], in[1], in[2]);
		
		write_outfile(out);
		if (out == '?' || out == '\0') cont = 0;
	}
	write_outfile('\n');
	
	close_infile();
	close_outfile();
}

void stdin_fileout_loop() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;	
	open_outfile();
	
	while (cont == 1) {
		in[0] = read_stdin();
		in[1] = read_stdin();
		in[2] = read_stdin();		
		
		char out = bioseq_codon_protein(in[0], in[1], in[2]);
		
		write_outfile(out);
		if (out == '?' || out == '\0') cont = 0;
	}
	write_outfile('\n');
	
	close_outfile();
}

// Parse args and set flags and values
int parseargs(int argc, char** argv) {
	int option = 0;
	char arg;
	
    while ((arg = getopt(argc, argv, "b:ho:pv")) != -1) {
        switch (arg) {
			
			case 'b':
				buffer_size = atoi(optarg);
			
			case 'o': 
				outfilestring = strdup(optarg);
				option = option | OUTFILE_F; 
				break;
				
			case 'p':
				option = option | PROTEIN_F;
				break;
				
			case 'v':
				print_version(); exit(0);
					
            case 'h':            
			default: print_help(); exit(1);
        }
	}

    if (argv[optind] != NULL) {
		
        if (optind + 1 == argc) { // One additional argument after options
			infilestring = argv[optind];
		}
		else {
			puts("Error: too many input files. Please supply one.");			
			exit(1);
		}
    }
    	
	return option;
}

void cleanup() {
    //free(outfilestring);
}

int main(int argc, char** argv) {
    
    if (argc > 1) {
		flags = parseargs(argc, argv);

	}	
    else print_help();
       
    if (infilestring == NULL && outfilestring == NULL) stdin_stdout_loop();
	if (infilestring != NULL && outfilestring == NULL) filein_stdout_loop();
	if (infilestring != NULL && outfilestring != NULL) filein_fileout_loop();
	if (infilestring == NULL && outfilestring != NULL) stdin_fileout_loop();
	
    cleanup();
    return 0;
}
