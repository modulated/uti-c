
CC=gcc
CFLAGS=-std=c99 -Wall -g

sieve: sieve.c bitarray.c
    $(CC) $(CCFLAGS) sieve.o bitarray.o

