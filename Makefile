CC=gcc
CFLAGS=-g -O0 -std=c99 -Wall -pedantic -Werror -Wno-unused-value -Wno-unused-function -I$(HEADER_DIR) -Ideps -D_GNU_SOURCE 
LDLIBS=$(TARGET) $(DIFF) -lm
PREFIX?=/usr/local

HEADER_DIR=include
SOURCES=$(wildcard src/libuti/*.c)
HEADERS=$(wildcard include/*.h)
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
$(TARGET): build $(OBJECTS) $(DIFF) $(SOURCES)
	ar rcs $@ $(OBJECTS)

build:
	@mkdir -p build

# The Unit Tests

.PHONY: tests
tests: LDLIBS += $(TEST_LIB)
tests: $(TEST_SRC) $(TARGET)
tests: $(TESTS)
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

seqc: $(TARGET) 
	$(CC) $(CFLAGS) -o bin/seqc.test src/seqc.c $(LDLIBS)


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
