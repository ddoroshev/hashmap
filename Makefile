BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests
PLAYGROUND_TARGET = ./bin/playground

CXX = g++-12
CFLAGS = -I. -Wall

OBJS =
TEST_OBJS = tests/test.o
MAIN_OBJS = main.o
PLAYGROUND_OBJS = playground.o
REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(PLAYGROUND_OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	$(CXX) $(CFLAGS) -o $@ $^

test-build: CFLAGS += -g
test-build: $(TESTS_TARGET)

test: test-build
	./bin/tests

cov: CFLAGS += -fprofile-arcs -ftest-coverage
cov: clean test
	lcov --directory . --capture --output-file info.cov
	mkdir -p cov-report
	genhtml -o cov-report info.cov

$(TESTS_TARGET): $(TEST_OBJS) $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^

$(PLAYGROUND_TARGET): $(PLAYGROUND_OBJS)
	$(CXX) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

tests/test.o: tests/assert.h hashmap/hashmap.hpp tests/assert.h
hashmap/test_array.o: tests/assert.h
hashmap/test_hashmap.o: hashmap/hashmap.hpp tests/assert.h
main.o: hashmap/hashmap.hpp
playground.o: hashmap/hashmap.hpp

playground: CFLAGS += -g
playground: $(PLAYGROUND_TARGET)
	./bin/playground

docker-test:
	docker build -t hashmap-test-debug -f Dockerfile .
	docker run -it --rm hashmap-test-debug make CXX=g++ test

docker-test-debug:
	docker build -t hashmap-test-debug -f Dockerfile.debug .
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it --rm hashmap-test-debug

docker-playground-debug:
	docker build -t hashmap-playground-debug -f Dockerfile.debug .
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it --rm hashmap-playground-debug gdb bin/playground

valgrind-build:
	docker build -t hashmap-valgrind-base -f Dockerfile.valgrind-base .

valgrind:
	docker build -t hashmap-valgrind -f Dockerfile.valgrind .
	docker run -it --rm hashmap-valgrind

clean:
	rm -rf $(REBUILDABLES) $(TESTS_TARGET) $(PLAYGROUND_TARGET) **/*.gc* *.cov cov-report
