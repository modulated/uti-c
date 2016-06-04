#include <stdio.h>
#include <stdlib.h>
#include "bioseq.h"
#include <getopt.h>
#include <string.h>

// Version
#define VERSION "0.0.3"

// Flag defines
#define RNA_F 1<<6
#define DNA_F 1<<5
#define PROTEIN_F 1<<4
#define BUFFER_F 1<<2
#define OUTFILE_F 1<<3
#define STDINPUT_F 1

int flags;
FILE* filep = NULL;
char* filestr;
unsigned buffer_size = 1000; 

void print_help(void) {
    puts(        
        "Usage: seqc [-hsv] [-b buffer size] [-o output] [file]\n"
		"Arguments:\n"
		"-h \tShow this message\n"
		"-s \tRead from standard input\n"
		"-v \tPrint version info.\n"
	);
}

void print_version(void) {
    puts(
        "seqc bioinformatics tool "VERSION"\n"
		"written by modulated"
     );
}

char read_stdin() {
    char in;
    while ((in = getc(stdin)) != EOF) {
        return in;
    }
    return '\0';
}

char read_infile(char* infiles) {
    
    FILE* infilep = NULL;
    infilep = fopen(infiles, "r");
    char in;
    while ((in = getc(infilep)) != EOF) {
       return in;
    }
    return '\0'; 
}
int parseargs(int argc, char** argv) {
	int option = 0;
	char arg;
	
    while ((arg = getopt(argc, argv, "b:hso:v")) != -1) {
        switch (arg) {
			
			case 'b':
				buffer_size = atoi(optarg);
			
			case 'o': 
				filestr = strdup(optarg);
				option = option | OUTFILE_F; 
				break;
				
            case 's': 
				option = option | STDINPUT_F; 
				break;
			
			case 'v':
				print_version(); exit(0);
					
            case 'h':            
			default: print_help(); exit(1);
        }
	}	
	return option;
}

int main(int argc, char** argv) {
    
    if (argc > 1) {
		flags = parseargs(argc, argv);
		if ((flags & OUTFILE_F) == 0) 
		printf("Option: %d\n",flags & OUTFILE_F);
	}	
    else print_help();
    
    if (flags & OUTFILE_F) puts("save to file\n");
    if (flags & STDINPUT_F) puts("Reading from stdin:\n");

    return 0;
}
