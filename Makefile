BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests
PLAYGROUND_TARGET = ./bin/playground
BENCHMARK_TARGET = ./bin/benchmark

CC = gcc
CFLAGS = -I. -Wall -std=gnu11 -fno-omit-frame-pointer
LDFLAGS =

OBJS = alloc.o hashmap.o dump.o
TEST_OBJS = test_hashmap.o test.o
MAIN_OBJS = main.o
PLAYGROUND_OBJS = playground.o $(OBJS)
BENCHMARK_OBJS = benchmark.o $(OBJS)

REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(PLAYGROUND_OBJS) $(BENCHMARK_OBJS) $(TARGET)

RELEASE_TARGET = $(TARGET)_release
RELEASE_BENCHMARK = $(BENCHMARK_TARGET)_release

RELEASE_CFLAGS = $(CFLAGS) -O3 -DNDEBUG
RELEASE_LDFLAGS = $(LDFLAGS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: test-build
test-build: CFLAGS += -DUNIT_TEST -fsanitize=address,undefined \
            -Wno-implicit-function-declaration -g
test-build: LDFLAGS += -fsanitize=address,undefined
test-build: $(TESTS_TARGET)

.PHONY: test
test: test-build
	./bin/tests

.PHONY: cov
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

.PHONY: release
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

.PHONY: playground
playground: CFLAGS += -g
playground: $(PLAYGROUND_TARGET)
	./bin/playground

.PHONY: benchmark-build
benchmark-build: CFLAGS += -g -fsanitize=address,undefined
benchmark-build: LDFLAGS += -fsanitize=address,undefined
benchmark-build: $(BENCHMARK_TARGET)

.PHONY: benchmark
benchmark: benchmark-build
	./bin/benchmark

.PHONY: docker-test-debug
docker-test-debug:
	docker build -t hashmap-test-debug -f docker/Dockerfile.debug .
	docker run -it --rm hashmap-test-debug

.PHONY: docker-playground-debug
docker-playground-debug:
	docker build -t hashmap-playground-debug -f docker/Dockerfile.debug .
	docker run -it --rm hashmap-playground-debug gdb bin/playground

.PHONY: valgrind
valgrind:
	docker build -t hashmap-valgrind -f docker/Dockerfile.valgrind .
	docker run -it --rm hashmap-valgrind

.PHONY: check
check:
	cppcheck \
		--language=c \
		-UUNIT_TEST \
		--safety \
		--inline-suppr \
		--enable=all \
		--check-level=exhaustive \
		--suppress=missingIncludeSystem \
		--suppress=constParameterPointer \
		--suppress=constVariablePointer \
		--suppress=variableScope \
		.

.PHONY: clean
clean:
	rm -rf $(REBUILDABLES) $(TESTS_TARGET) $(RELEASE_TARGET) $(RELEASE_BENCHMARK) **.gc* *.cov cov-report
