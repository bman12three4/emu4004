#include <stdio.h>

#include "4001.h"
#include "4002.h"
#include "4004.h"
#include "test.h"

int main(void)
{
	FILE* program = fopen("subroutinetest.mcs4", "r");

	struct cpu_4004 *cpu = create_cpu();
	struct memory_node *rom = create_rom(program);

	attach_rom(cpu, rom, 0);

	for (int i = 0; i < 10; i++) {
		excecute_cpu(cpu);
		getchar();
	}
	destroy_cpu(cpu);
}
