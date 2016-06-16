CC=clang
CFLAGS=-g -Wall -pedantic -Werror -Wno-unused-function -Isrc -D_GNU_SOURCE
LIBS=-ldl -lm
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_LIB=tests/tap.c
TEST_SRC=$(wildcard tests/*.test.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libuti.a

# The Target Build
all: $(TARGET) tests

prod: CFLAGS=-O2 -Wall -Isrc -Wall -Wextra
prod: all

$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@	

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(TARGET) $(TEST_LIB)
tests: $(TESTS)	
	sh ./tests/runall.sh

valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all	
	install $(TARGET) $(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true