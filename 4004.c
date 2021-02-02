#include <stdlib.h>
#include "4004.h"

struct cpu_4004 * create_cpu()
{
	struct cpu_4004 *cpu = malloc(sizeof(struct cpu_4004));
	reset_cpu(cpu);
	return cpu;
}

void reset_cpu(struct cpu_4004* cpu)
{

	cpu->accumulator = 0;
	cpu->cond = 0;

	for (int i = 0; i < 8; i++){
		cpu->regp[i] = 0;
	}

	cpu->pc = 0;

	for (int i = 0; i < 3; i++){
		cpu->stack[i] = 0;
	}

	cpu->mem = malloc(sizeof(struct mem_4001));
	for (int i = 0; i < 256; i++){
		cpu->mem->rom[i] = 0;
	}
}

void destroy_cpu(struct cpu_4004* cpu){
	free(cpu->mem);
	free(cpu);
}