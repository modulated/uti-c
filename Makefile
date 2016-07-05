CC=gcc
CFLAGS=-g -std=c99 -Wall -pedantic -Werror -Wno-unused-value -Wno-unused-function -Isrc -Ideps -D_GNU_SOURCE 
LDLIBS=-lm $(TARGET) $(DIFF)
PREFIX?=/usr/local

SOURCES=$(wildcard src/libuti/*.c)
HEADERS=$(wildcard src/libuti/*.h)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

DIFF=deps/libdmp.a
DIFF_SRC=deps/dmp.c deps/dmp_pool.c
DIFF_OBJ=$(patsubst %.c,%.o,$(DIFF_SRC))

TEST_LIB=deps/tap.c
TEST_SRC=$(wildcard tests/*.test.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libuti.a

# The Target Build
all: $(TARGET) tests

# Archive built objects to TARGET
$(TARGET): build $(OBJECTS) $(DIFF)
	ar rcs $@ $(OBJECTS)

build:
	@mkdir -p build

# The Unit Tests

.PHONY: tests
tests: LDLIBS += $(TEST_LIB)
tests: $(TESTS)
	# sh ./tests/runall.sh
	@for f in tests/*.test; \
	do \
		$$f; \
		if [ $$? -ne 0 ]; \
			then exit; \
		fi; \
	done;

$(DIFF): $(DIFF_OBJ)
	ar rcs $@ $(DIFF_OBJ)

valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS) $(DIFF) $(DIFF_OBJ)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all	
	install $(TARGET) $(PREFIX)/lib/
	cp $(HEADERS) $(PREFIX)/include/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true
