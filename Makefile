CC=clang
CFLAGS=-Wall -Werror -lm -D_GNU_SOURCE -g -O0
TESTLIB=$(TESTDIR)/tap.c

LIBDIR=lib
BINDIR=bin
SRCDIR=src
TESTDIR=test


all:
	make test

seqc:
	$(CC) $(CFLAGS) -o seqc src/seqc.c src/bioseq.c
	
test:
	mkdir -p ./bin
	make bio
	make random
	make stats	
	$(BINDIR)/bio
	$(BINDIR)/random
	$(BINDIR)/stats

# lib: $(SRCDIR)/bioseq.c
# 	$(CC) $(CFLAGS) -c $(SRCDIR)/bioseq.c -o bioseq.o
# 	libtool bioseq.o -o $(LIBDIR)/libbio.a
# 	rm bioseq.o
	
bio: $(TESTDIR)/bioseq.test.c $(SRCDIR)/bioseq*.c
	$(CC) $(CFLAGS) $(SRCDIR)/bioseq.c $(TESTDIR)/bioseq.test.c -o $(BINDIR)/bio $(TESTLIB)
	
	
random: $(TESTDIR)/random.test.c $(SRCDIR)/random.c
	$(CC) $(CFLAGS) $(SRCDIR)/random.c $(TESTDIR)/random.test.c -o $(BINDIR)/random $(TESTLIB)
	

stats: $(TESTDIR)/stats.test.c $(SRCDIR)/stats.c
	$(CC) $(CFLAGS) $(SRCDIR)/stats.c $(TESTDIR)/stats.test.c -o $(BINDIR)/stats $(TESTLIB)
	

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib
	
clean:
	rm -rf ./bin/*
