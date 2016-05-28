CC=clang
CFLAGS=-Wall -Werror -pedantic -std=c99
COMPILELIBS=-lm
TESTFILES=$(TESTDIR)/tap.c

LIBDIR=lib
BINDIR=bin
SRCDIR=src
TESTDIR=tests


all: $(SRCDIR)/*.c
	make test

seqc: $(SRCDIR)/seqc.c
	$(CC) $(CFLAGS) -o bin/seqc src/seqc.c

test:
	mkdir -p ./bin
	make bio
	make random
	make stats
	

# lib: $(SRCDIR)/bioseq.c
# 	$(CC) $(CFLAGS) -c $(SRCDIR)/bioseq.c -o bioseq.o
# 	libtool bioseq.o -o $(LIBDIR)/libbio.a
# 	rm bioseq.o
	
bio: $(TESTDIR)/bioseq.test.c $(SRCDIR)/bioseq.c
	$(CC) $(CFLAGS) $(SRCDIR)/bioseq.c $(TESTDIR)/bioseq.test.c -o $(BINDIR)/bio $(TESTFILES)
	$(BINDIR)/bio | faucet
	
random: $(TESTDIR)/random.test.c $(SRCDIR)/random.c
	$(CC) $(CFLAGS) $(SRCDIR)/random.c $(TESTDIR)/random.test.c -o $(BINDIR)/random $(TESTFILES)
	$(BINDIR)/random | faucet

stats: $(TESTDIR)/stats.test.c $(SRCDIR)/stats.c
	$(CC) $(CFLAGS) $(SRCDIR)/stats.c $(TESTDIR)/stats.test.c -o $(BINDIR)/stats $(TESTFILES)
	$(BINDIR)/stats | faucet

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib
	
clean:
	rm -rf ./bin/*
