#include <stdio.h>
#include "4004.h"

int main(void)
{
    struct cpu_4004 *cpu = create_cpu();
    printf("cpu pointer: %p\n", cpu);
    destroy_cpu(cpu);
}
