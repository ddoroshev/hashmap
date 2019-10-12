BIN = ./bin/
TARGET = ./bin/main
TESTS_TARGET = ./bin/tests

OBJS = alloc.o array.o hashmap.o
TEST_OBJS = tests.o
MAIN_OBJS = main.o
REBUILDABLES = $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJS)
	gcc -o $@ $^

test: $(TESTS_TARGET)
	./bin/tests

$(TESTS_TARGET): $(TEST_OBJS) $(OBJS)
	gcc -o $@ $^

%.o: %.c
	gcc -o $@ -c $<

main.o: hashmap.h array.h alloc.h
hashmap.o: hashmap.h array.h alloc.h
array.o: array.h alloc.h
tests.o: hashmap.h array.h alloc.h

clean:
	rm -f $(REBUILDABLES)
