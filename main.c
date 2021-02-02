#include <stdio.h>
#include "4004.h"
#include "test.h"

int main(void)
{
	struct cpu_4004 *cpu = create_cpu();
	test_all(cpu);
	destroy_cpu(cpu);
}
