BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests

CC = gcc
CFLAGS = -I. -Wall -std=c11

OBJS = alloc.o array.o hashmap.o dump.o
TEST_OBJS = tests/test_array.o tests/test_hashmap.o tests/test.o
MAIN_OBJS = main.o
REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test: CFLAGS += -Wno-implicit-function-declaration
test: $(TESTS_TARGET)
	./bin/tests

cov: CFLAGS += -fprofile-arcs -ftest-coverage
cov: test
	lcov --directory . --capture --output-file info.cov
	mkdir -p cov-report
	genhtml -o cov-report info.cov

$(TESTS_TARGET): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

tests/test.o: tests/assert.h
tests/test_array.o: array.h alloc.h hashmap_item.h tests/assert.h
tests/test_hashmap.o: hashmap.h alloc.h hashmap_item.h tests/assert.h
main.o: hashmap.h array.h alloc.h hashmap_item.h
hashmap.o: hashmap.h array.h alloc.h hashmap_item.h
array.o: array.h alloc.h
dump.o: array.h hashmap.h hashmap_item.h

clean:
	rm -rf $(REBUILDABLES) $(TESTS_TARGET) *.gc* tests/*.gc* *.cov cov-report
