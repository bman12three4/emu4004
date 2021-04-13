CC=gcc
CFLAGS=-I. -g -std=c99
DEPS = 4001.h 4002.h 4003.h 4004.h test.h
OBJ = main.o 4001.o 4002.o 4003.o 4004.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: emu4004 test assembler

emu4004: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

test: test.o
	$(CC) -o $@ $^ $(CFLAGS)

assembler: assembler.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf *.o emu4004 assembler