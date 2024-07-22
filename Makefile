BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests
PLAYGROUND_TARGET = ./bin/playground
BENCHMARK_TARGET = ./bin/benchmark

CC = gcc
CFLAGS = -I. -Wall -std=gnu11 -fno-omit-frame-pointer
LDFLAGS =

OBJS = alloc.o array.o hashmap.o dump.o
TEST_OBJS = test_hashmap.o test.o
MAIN_OBJS = main.o
PLAYGROUND_OBJS = playground.o
BENCHMARK_OBJS = benchmark.o $(OBJS)

REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(PLAYGROUND_OBJS) $(BENCHMARK_OBJS) $(TARGET)

RELEASE_TARGET = $(TARGET)_release
RELEASE_BENCHMARK = $(BENCHMARK_TARGET)_release

RELEASE_CFLAGS = $(CFLAGS) -O3 -DNDEBUG
RELEASE_LDFLAGS = $(LDFLAGS)

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

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
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BENCHMARK_TARGET): $(BENCHMARK_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

release: CFLAGS := $(RELEASE_CFLAGS)
release: LDFLAGS := $(RELEASE_LDFLAGS)
release: clean $(RELEASE_TARGET) $(RELEASE_BENCHMARK)

$(RELEASE_TARGET): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	strip $@

$(RELEASE_BENCHMARK): $(BENCHMARK_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	strip $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

*.o: *.h

playground: CFLAGS += -g
playground: $(PLAYGROUND_TARGET)
	./bin/playground

benchmark-build: CFLAGS += -g
benchmark-build: $(BENCHMARK_TARGET)

benchmark: benchmark-build
	./bin/benchmark

docker-test-debug:
	docker build -t hashmap-test-debug -f docker/Dockerfile.debug .
	docker run -it --rm hashmap-test-debug

docker-playground-debug:
	docker build -t hashmap-playground-debug -f docker/Dockerfile.debug .
	docker run -it --rm hashmap-playground-debug gdb bin/playground

valgrind:
	docker build -t hashmap-valgrind -f docker/Dockerfile.valgrind .
	docker run -it --rm hashmap-valgrind

clean:
	rm -rf $(REBUILDABLES) $(TESTS_TARGET) $(RELEASE_TARGET) $(RELEASE_BENCHMARK) **.gc* *.cov cov-report
