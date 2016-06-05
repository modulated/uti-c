CC=clang
CFLAGS=-Wall -Werror -D_GNU_SOURCE
COMPILELIBS=-lm
TESTLIB=$(TESTDIR)/tap.c

LIBDIR=lib
BINDIR=bin
SRCDIR=src
TESTDIR=test


all: $(SRCDIR)/*.c
	make test

seqc: $(SRCDIR)/seqc.c $(SRCDIR)/bioseq*.c
	$(CC) $(CFLAGS) -o seqc src/seqc.c src/bioseq.c

test: $(SRCDIR)/* $(TESTDIR)/*
	mkdir -p ./bin
	make bio
	make random
	make stats
	

# lib: $(SRCDIR)/bioseq.c
# 	$(CC) $(CFLAGS) -c $(SRCDIR)/bioseq.c -o bioseq.o
# 	libtool bioseq.o -o $(LIBDIR)/libbio.a
# 	rm bioseq.o
	
bio: $(TESTDIR)/bioseq.test.c $(SRCDIR)/bioseq*.c
	$(CC) $(CFLAGS) $(SRCDIR)/bioseq.c $(TESTDIR)/bioseq.test.c -o $(BINDIR)/bio $(TESTLIB)
	$(BINDIR)/bio
	
random: $(TESTDIR)/random.test.c $(SRCDIR)/random.c
	$(CC) $(CFLAGS) $(SRCDIR)/random.c $(TESTDIR)/random.test.c -o $(BINDIR)/random $(TESTLIB)
	$(BINDIR)/random

stats: $(TESTDIR)/stats.test.c $(SRCDIR)/stats.c
	$(CC) $(CFLAGS) $(SRCDIR)/stats.c $(TESTDIR)/stats.test.c -o $(BINDIR)/stats $(TESTLIB)
	$(BINDIR)/stats

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib
	
clean:
	rm -rf ./bin/*
