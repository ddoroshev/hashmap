BIN = ./bin/
TARGET = ./bin/main

OBJS = array.o hashmap.o main.o
REBUILDABLES = $(OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $^

%.o: %.c
	gcc -o $@ -c $<

main.o: hashmap.h array.h
hashmap.o: hashmap.h array.h
array.o: array.h

clean:
	rm -f $(REBUILDABLES)
