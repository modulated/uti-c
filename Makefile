CC=clang
CFLAGS=-Wall -Werror
LIBS=-lm

LIBDIR=lib
SRCDIR=src
TESTDIR=tests


all: $(OBJS)
	$(CC) $(CFLAGS) -o bin/% $(OBJS) $(LIBS)

bio: $(SRCDIR)/bioseq.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/bioseq.c -o bioseq.o
	libtool bioseq.o -o $(LIBDIR)/libbio.a
	rm bioseq.o
	
test: $(TESTDIR)/*.test.c
	$(CC) $(CFLAGS) $(TESTDIR)/*.test.c -o test.out -L${pwd}/$(LIBDIR) -lbio -ltap

install: $(LIBDIR)/*.a
	install $(LIBDIR)/*.a /usr/local/lib