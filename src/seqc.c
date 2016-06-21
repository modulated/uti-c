#include <stdio.h>
#include <stdlib.h>
#include "bioseq.h"
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "color.h"

// Version
#define VERSION "0.1.6"

// Flag defines
#define ELECTRO_F 0x1<<6
#define DIFF_F 0x1<<5
#define READ_F 0x1<<4
#define STATS_F 0x1<<3
#define OUTFILE_F 0x1<<2
#define PROTEIN_F 0x1

// Global vars
static int flags;
static FILE* infilepointer = NULL;
static FILE* infilepointer2 = NULL;
static FILE* outfilepointer = NULL;
static char* infilestring = NULL;
static char* infilestring2 = NULL;
static char* outfilestring = NULL;
static bioseq_stats stats;

void print_help(void) {
    puts(        
        "Usage: seqc [-dhvp] [-o output] file1 ...\n"
		"Arguments:\n"
		"-d \tDiff DNA sequences.\n"
		"-h \tShow this message\n"
		"-v \tPrint version info.\n"
		"-p \tConvert to protein.\n"
		"-s \tGet DNA statistics.\n"
	);
}

void print_version(void) {
    puts(
        "seqc bioinformatics tool "VERSION"\n"
		"written by modulated"
     );
}

void print_stats(void) {
	int spacer_int = 16;
	int spacer_float = 16;
	
	puts(COL_BLU"Statistical Analysis of DNA:"COL_RST);
	puts(COL_YEL"\t\t\tA\t\t C\t\t  G\t\t   T\t   Total(GC%)"COL_RST);
	printf("Count:   %*lu %*lu %*lu %*lu %*llu\n", spacer_int, stats.a_count, spacer_int, stats.c_count, spacer_int, stats.g_count, spacer_int, stats.t_count, spacer_int, stats.total_count);
	printf("Percent: %*.2f %*.2f %*.2f %*.2f %*.2f\n", spacer_float, stats.a_percent, spacer_float, stats.c_percent, spacer_float, stats.g_percent, spacer_float, stats.t_percent, spacer_float, stats.gc_percent);
}

/*
* File Handling Functions
*/

size_t file_size(FILE* file) {
	
	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	return size;
}

void open_infile() {
    infilepointer = fopen(infilestring, "r");	
	
	if (infilepointer == NULL) {
		puts("Error: cannot access input file.");
		printf("%s\n",strerror(errno));
		exit(errno);
	}
	
	flockfile(infilepointer);
}

void open_infile2() {
    infilepointer2 = fopen(infilestring2, "r");	
	
	if (infilepointer2 == NULL) {
		puts("Error: cannot access input file.");
		printf("%s\n",strerror(errno));
		exit(errno);
	}
	
	flockfile(infilepointer);
}

void open_outfile() {
    outfilepointer = fopen(outfilestring, "w");
	
	if (outfilepointer == NULL) {
		puts("Error: cannot access output file.");
		printf("%s\n",strerror(errno));
		exit(errno);
	}
	
	flockfile(outfilepointer);
}

void close_infile() {	
	fclose(infilepointer);
	funlockfile(infilepointer);
}

void close_outfile() {	
	fclose(outfilepointer);
	funlockfile(outfilepointer);
}

char read_stdin() {
    char in = getchar();
    if (in == EOF) {
        return '\0';
    }
	in = toupper(in);
	
    if (NUCLEOTIDE_SANITIZE(in)) {
		bioseq_stats_stream(&stats, in);
		return in;
	}
	
	else {
		return read_stdin();
	}
}

char read_infile() {
    char in = getc_unlocked(infilepointer);
    if (in == EOF) {
        return '\0';
    }
	in = toupper(in);
	
	if (NUCLEOTIDE_SANITIZE(in)) {
		bioseq_stats_stream(&stats, in);
		return in;
	}
	
	else {
		return read_infile();
	}
}

void write_outfile(char out) {
	if (putc_unlocked(out, outfilepointer) == EOF)
	{
		puts("Error: cannot write to file.");
		exit(1);
	}
}

/*
** Protein Loop Functions
*/

void stdin_stdout_protein() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	
	while (cont == 1) {
		in[0] = read_stdin();
		in[1] = read_stdin();
		in[2] = read_stdin();		
		
		char out = bioseq_dna_codon_protein(in[0], in[1], in[2]);
		
		if(putchar(out) == EOF) {
			puts("Error: cannot write to stdout.");			
			exit(1);
		}
		if (out == '?' || out == '\0') cont = 0;
	}
	putchar('\n');
}

void filein_stdout_protein() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	open_infile();
	
	while (cont == 1) {
		in[0] = read_infile();
		in[1] = read_infile();
		in[2] = read_infile();		
		
		char out = bioseq_dna_codon_protein(in[0], in[1], in[2]);
		
		fputc(out, stdout);
		if (out == '?' || out == '\0') cont = 0;
	}
	puts("");
	
	close_infile();
}

void filein_stats_protein() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	open_infile();
	
	while (cont == 1) {
		in[0] = read_infile();
		in[1] = read_infile();
		in[2] = read_infile();		
		
		char out = bioseq_dna_codon_protein(in[0], in[1], in[2]);

		if (out == '?' || out == '\0') cont = 0;
	}
	puts("");
	
	close_infile();
}

void stdin_stats_protein() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	
	while (cont == 1) {
		in[0] = read_stdin();
		in[1] = read_stdin();
		in[2] = read_stdin();		
		
		char out = bioseq_dna_codon_protein(in[0], in[1], in[2]);

		if (out == '?' || out == '\0') cont = 0;
	}
	puts("");
}

void filein_fileout_protein() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;
	open_infile();
	open_outfile();
	
	while (cont == 1) {
		in[0] = read_infile();
		in[1] = read_infile();
		in[2] = read_infile();		
		
		char out = bioseq_dna_codon_protein(in[0], in[1], in[2]);
		
		write_outfile(out);
		if (out == '?' || out == '\0') cont = 0;
	}
	write_outfile('\n');
	
	close_infile();
	close_outfile();
}

void stdin_fileout_protein() {
	char in[3] = {'\0','\0','\0'};
	int cont = 1;	
	open_outfile();
	
	while (cont == 1) {
		in[0] = read_stdin();
		in[1] = read_stdin();
		in[2] = read_stdin();		
		
		char out = bioseq_dna_codon_protein(in[0], in[1], in[2]);
		
		write_outfile(out);
		if (out == '?' || out == '\0') cont = 0;
	}
	write_outfile('\n');
	
	close_outfile();
}

void fileout_diff() {
	
}

void stdout_diff() {
	
	open_infile();
	open_infile2();

	size_t insize = file_size(infilepointer);
	size_t insize2 = file_size(infilepointer2);

	printf("%s: %lu \t %s: %lu\n", infilestring, insize, infilestring2, insize2);

	char* bufferfile1 = malloc((insize + 1) * sizeof(char));
	char* bufferfile2 = malloc((insize2 + 1) * sizeof(char));

	fread (bufferfile1, 1, insize, infilepointer);
	fread (bufferfile2, 1, insize2, infilepointer2);

	int similarity = bioseq_string_similarity(bufferfile1, bufferfile2);
	int insertions = bioseq_string_insertions(bufferfile1, bufferfile2);
	int deletions = bioseq_string_deletions(bufferfile1, bufferfile2);

	printf("Similarity: %d\n", similarity);
	printf("Insertions: %d\n", insertions);
	printf("Deletions: %d\n", deletions);
}

void seq_diff() {
	if (infilestring == NULL || infilestring2 == NULL) {
		puts("Please supply two files.");
		exit(1);
	}
	if (outfilestring != NULL) fileout_diff();
	if (outfilestring == NULL) stdout_diff();
}

void seq_protein() {
	if (infilestring == NULL && outfilestring == NULL) stdin_stdout_protein();
	if (infilestring != NULL && outfilestring == NULL) filein_stdout_protein();
	if (infilestring != NULL && outfilestring != NULL) filein_fileout_protein();
	if (infilestring == NULL && outfilestring != NULL) stdin_fileout_protein();
	
	flags = flags | READ_F;
}

void seq_stats() {
		
		if (!(flags & READ_F)) {
			if (infilestring == NULL) stdin_stats_protein();
			else filein_stats_protein();
		}
		
		bioseq_stats_calculate(&stats);
		print_stats();	
}

void seq_determine() {
	if (flags & PROTEIN_F) {
		seq_protein();
	}
	
	if (flags & STATS_F) {
		seq_stats();
	}

	if (flags & DIFF_F) {
		seq_diff();
	}
}

// Parse args and set flags and values
int parseargs(int argc, char** argv) {
	int option = 0;
	char arg;
	
    while ((arg = getopt(argc, argv, "dho:psv")) != -1) {
        switch (arg) {
			
			case 'd': 
				option |= DIFF_F;
				break;

			case 'o': 
				outfilestring = strdup(optarg);
				break;
				
			case 'p':
				option |= PROTEIN_F;
				break;
				
			case 's':
				option |= STATS_F;
				break;
				
			case 'v':
				print_version(); exit(0);
					
            case 'h': default: 
				print_help(); exit(1);
        }
	}
	
    if (argv[optind] != NULL) { // If arg after last option exists
		
		// If -p
		if (option & PROTEIN_F) {
			if (optind + 1 == argc) { // One additional argument after options
				infilestring = argv[optind];
			}
			else {
				puts("Error: too many input files. Please supply one.");
				exit(1);
			}
		}

		// If -d
		if (option & DIFF_F) {
			if (optind + 2 == argc) { // Two additional argument after options
				infilestring = argv[optind];
				infilestring2 = argv[optind+1];
			}
			else {
				puts("Error: Please supply two input files.");
				exit(1);
			}
		}
    }
    	
	return option;
}

void cleanup() {
	free(outfilestring);
}

int main(int argc, char** argv) {
    
    if (argc > 1) {
		flags = parseargs(argc, argv);
	}	
    else print_help();
       
	seq_determine();
	
    cleanup();
    return 0;
}
