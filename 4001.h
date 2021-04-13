#include <stdlib.h>
#include "4004.h"

#ifndef _4001_H
#define _4001_H

struct rom_4001 {
	char data[256];
	char out;
	char in;
};

struct memory_node* create_rom(FILE* f);

void write_4001(struct cpu_4004* cpu, void* rom, unsigned short addr, unsigned char val);
unsigned char read_4001(struct cpu_4004* cpu, void* rom, unsigned short addr);
void destroy_4001();

#endif