#include <stdlib.h>

#ifndef _4001_H
#define _4001_H

struct rom_4001 {
	char data[256];
	char out;
	char in;
};

struct rom_4001* create_rom(FILE* f);
void destroy_rom(struct rom_4001*);

#endif