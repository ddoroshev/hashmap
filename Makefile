BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests
PLAYGROUND_TARGET = ./bin/playground

CC = gcc
CFLAGS = -I. -Wall -std=c11

OBJS = alloc/alloc.o array/array.o hashmap/hashmap.o dump.o
TEST_OBJS = array/test_array.o hashmap/test_hashmap.o tests/test.o
MAIN_OBJS = main.o
PLAYGROUND_OBJS = playground.o
REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(PLAYGROUND_OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test-build: CFLAGS += -Wno-implicit-function-declaration -g
test-build: $(TESTS_TARGET)

test: test-build
	./bin/tests

cov: CFLAGS += -fprofile-arcs -ftest-coverage
cov: clean test
	lcov --directory . --capture --output-file info.cov
	mkdir -p cov-report
	genhtml -o cov-report info.cov

$(TESTS_TARGET): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(PLAYGROUND_TARGET): $(PLAYGROUND_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

tests/test.o: tests/assert.h
array/test_array.o: array/array.h alloc/alloc.h hashmap/hashmap_item.h tests/assert.h
hashmap/test_hashmap.o: hashmap/hashmap.h alloc/alloc.h hashmap/hashmap_item.h tests/assert.h
main.o: hashmap/hashmap.h array/array.h alloc/alloc.h hashmap/hashmap_item.h
hashmap.o: hashmap/hashmap.h array/array.h alloc/alloc.h hashmap/hashmap_item.h
array.o: array/array.h alloc/alloc.h
dump.o: array/array.h hashmap/hashmap.h hashmap/hashmap_item.h

playground: CFLAGS += -g
playground: $(PLAYGROUND_TARGET)
	./bin/playground

docker-test-debug:
	docker build -t hashmap-test-debug -f Dockerfile.debug .
	docker run -it --rm hashmap-test-debug

docker-playground-debug:
	docker build -t hashmap-playground-debug -f Dockerfile.debug .
	docker run -it --rm hashmap-playground-debug gdb bin/playground

clean:
	rm -rf $(REBUILDABLES) $(TESTS_TARGET) **/*.gc* *.cov cov-report
