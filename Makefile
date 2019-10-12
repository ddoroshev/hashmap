BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests

CC = gcc
CFLAGS = -I.

OBJS = alloc.o array.o hashmap.o
TEST_OBJS = tests/test_array.o tests/test.o
MAIN_OBJS = main.o
REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test: CFLAGS += -Wno-implicit-function-declaration
test: $(TESTS_TARGET)
	./bin/tests

$(TESTS_TARGET): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

tests/test_array.o: array.h alloc.h tests/assert.h
main.o: hashmap.h array.h alloc.h
hashmap.o: hashmap.h array.h alloc.h
array.o: array.h alloc.h

clean:
	rm -f $(REBUILDABLES)
