CC=clang
CFLAGS=-Wall -Werror
COMPILELIBS=-lm
TESTLIBS=-Llib/ -ltap

LIBDIR=lib
SRCDIR=src
TESTDIR=tests


all: $(SRCDIR)/*.c
	make bio;
	make random;
	make stats;
	

lib: $(SRCDIR)/bioseq.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/bioseq.c -o bioseq.o
	libtool bioseq.o -o $(LIBDIR)/libbio.a
	rm bioseq.o
	
bio: $(TESTDIR)/bioseq.test.c
	$(CC) $(CFLAGS) $(SRCDIR)/bioseq.c $(TESTDIR)/bioseq.test.c -o bio $(TESTLIBS)
	./bio | faucet
	
random: $(TESTDIR)/random.test.c
	$(CC) $(CFLAGS) $(SRCDIR)/random.c $(TESTDIR)/random.test.c -o random $(TESTLIBS)
	./random | faucet

stats: $(TESTDIR)/stats.test.c
	$(CC) $(CFLAGS) $(SRCDIR)/stats.c $(TESTDIR)/stats.test.c -o stats $(TESTLIBS)
	-./stats | faucet

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib