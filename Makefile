CC = gcc
CFLAGS = -Wall
LIBS = -lm

SRC = $(shell find . -name 'src/*.c')
OBJS = $(addprefix ./,$(SRC:%.c=%.o))

make: $(OBJS)
	$(CC) $(CFLAGS) -o bin/% $(OBJS) $(LIBS)
