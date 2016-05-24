CC=clang
CFLAGS=-Wall -Werror
COMPILELIBS=-lm
TESTLIBS=-Llib/ -ltap

LIBDIR=lib
BINDIR=bin
SRCDIR=src
TESTDIR=tests


all: $(SRCDIR)/*.c
	make test

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
	$(CC) $(CFLAGS) $(SRCDIR)/bioseq.c $(TESTDIR)/bioseq.test.c -o $(BINDIR)/bio $(TESTLIBS)
	$(BINDIR)/bio | faucet
	
random: $(TESTDIR)/random.test.c $(SRCDIR)/random.c
	$(CC) $(CFLAGS) $(SRCDIR)/random.c $(TESTDIR)/random.test.c -o $(BINDIR)/random $(TESTLIBS)
	$(BINDIR)/random | faucet

stats: $(TESTDIR)/stats.test.c $(SRCDIR)/stats.c
	$(CC) $(CFLAGS) $(SRCDIR)/stats.c $(TESTDIR)/stats.test.c -o $(BINDIR)/stats $(TESTLIBS)
	$(BINDIR)/stats | faucet

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib
	
clean:
	rm -rf ./bin/*