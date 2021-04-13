#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "4001.h"
#include "4004.h"

struct memory_node * create_rom(FILE* file)
{
	struct memory_node *node = malloc(sizeof(struct memory_node));
	struct rom_4001 *rom = malloc(sizeof(struct rom_4001));

	node->device = rom;
	node->read = read_4001;
	node->write = write_4001;
	node->destroy = destroy_4001;

	if (!file) {
		file = fopen("./sample.mcs4", "r");
	}

	fread(rom->data, 1, 256, file);
	return node;
}

void write_4001(struct cpu_4004* cpu, void* rom, unsigned short addr, unsigned char val)
{
	((struct rom_4001*)rom)->data[addr] = val;
}

unsigned char read_4001(struct cpu_4004* cpu, void* rom, unsigned short addr)
{
	return ((struct rom_4001*)rom)->data[addr];
}
void destroy_4001(struct memory_node* node){
	free (node->device);
	free (node);
}