#include <stdlib.h>
#include "4004.h"

struct cpu_4004 * create_cpu()
{
	struct cpu_4004 *cpu;

	cpu = malloc(sizeof(struct cpu_4004));

	cpu->accumulator = 0;
	cpu->cond = 0;

	for (int i = 0; i < 8; i++){
		cpu->regp[i] = 0;
	}

	cpu->pc = 0;

	for (int i = 0; i < 3; i++){
		cpu->stack[i] = 0;
	}
}