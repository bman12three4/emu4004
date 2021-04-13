#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "4001.h"

struct rom_4001 * create_rom(FILE* file)
{
	struct rom_4001 *rom = malloc(sizeof(struct rom_4001));

	if (!file) {
		file = fopen("./sample.mcs4", "r");
	}

	fread(rom->data, 1, 256, file);
	return rom;
}

void destroy_rom(struct rom_4001* rom)
{
	free(rom);
}