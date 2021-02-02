CC=gcc
CFLAGS=-I. -g -std=c99
DEPS = 4004.h test.h
OBJ = main.o 4004.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

emu4004: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf *.o emu4004