BIN = ./bin/
TARGET = ./bin/main

OBJS = array.o hashmap.o main.o
REBUILDABLES = $(OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $^

%.o: %.c
	gcc -o $@ -c $<

clean:
	rm -f $(REBUILDABLES)
