#include <stdio.h>
#include <stdlib.h>
#include "bioseq.h"
#include <getopt.h>
#include <ctype.h>

void printhelp(void) {
    puts("seqc bioinformatics tool v0.a\nseqc [-psbrcdo] file");
}

int parseargs(int argc, char** argv) {
    int option = 0;
    while ((option = getopt(argc, argv, "hso:b:")) != -1) {
        switch (option) {
            
            case 'h': printhelp(); break;
            default: printhelp();
            exit(1);
        }
    }

    return option;
}

int main(int argc, char** argv) {
    int flags = 0;

    flags = parseargs(argc, argv);
    
    return 0;
}
