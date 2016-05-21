CC=clang
CFLAGS=-Wall -Werror
LIBS=-lm

LIBDIR=lib
SRCDIR=src
TESTDIR=tests


all: $(OBJS)
	$(CC) $(CFLAGS) -o bin/% $(OBJS) $(LIBS)

lib: $(SRCDIR)/bioseq.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/bioseq.c -o bioseq.o
	libtool bioseq.o -o $(LIBDIR)/libbio.a
	rm bioseq.o
	
bio: $(TESTDIR)/bioseq.test.c
	$(CC) $(CFLAGS) $(SRCDIR)/bioseq.c $(TESTDIR)/bioseq.test.c -o bio -ltap
	./bio | faucet
	
random: $(TESTDIR)/random.test.c
	$(CC) $(CFLAGS) $(SRCDIR)/random.c $(TESTDIR)/random.test.c -o random -ltap
	./random | faucet

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib