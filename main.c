#include <stdio.h>

#include "4001.h"
#include "4002.h"
#include "4004.h"
#include "test.h"

int main(void)
{
	struct cpu_4004 *cpu = create_cpu();
	struct rom_4001 *rom = create_rom(NULL);

	attach_rom(cpu, rom, 0);

	for (int i = 0; i < 10; i++) {
		excecute_cpu(cpu);
		getchar();
	}

	destroy_rom(rom);
	destroy_cpu(cpu);
}
