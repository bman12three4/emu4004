#include <stdlib.h>
#include <stdio.h>
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
	cpu->flags = 0;

	for (int i = 0; i < 8; i++){
		cpu->regp[i] = 0;
	}

	cpu->pc = 0;

	for (int i = 0; i < 3; i++){
		cpu->stack[i] = 0;
	}

	cpu->sp = 0;

	for (int i = 0; i < 256; i++){
		cpu->rom[i] = 0;
	}
}

void destroy_cpu(struct cpu_4004* cpu)
{
	free(cpu);
}

void excecute_cpu(struct cpu_4004* cpu)
{
	unsigned char opr = cpu->rom[cpu->pc++] & 0xf;
	unsigned char opa = cpu->rom[cpu->pc++] & 0xf;

	switch (opr){
	case NOP: {
		break;
	}
	case JCN: {
		int cc = ((cpu->test << 0) | (cpu->flags << 1) | ((cpu->accumulator ? 1 : 0) << 2));
		if ((opa & 0b111 & cc) || opa & 0b1000){
			unsigned char addrh = (cpu->rom[cpu->pc++]);
			unsigned char addrl = (cpu->rom[cpu->pc++]);
			cpu->pc = (addrh << 4) | addrl;
		}
		break;
	}
	case (FIM): {
		unsigned char reg = opa >> 1;
		unsigned char addrh = (cpu->rom[cpu->pc++]);
		unsigned char addrl = (cpu->rom[cpu->pc++]);

		cpu->regp[reg] = cpu->rom[(addrh << 4) | addrl];
		
		break;
	}
	case (SRC): {
		printf("Unimplemented Instruction.\n");
		break;
	}
	case (FIN): {
		cpu->regp[opa >> 1] = cpu->rom[cpu->regp[0]];
		break;
	}
	case (JIN): {
		cpu->pc = cpu->regp[opa >> 1];
		break;
	}
	case (JUN): {
		//unsigned char addrh = opa;
		// not used since there is only 1 rom right now
		unsigned char addrm = (cpu->rom[cpu->pc++]);
		unsigned char addrl = (cpu->rom[cpu->pc++]);

		cpu->pc = (addrm << 4) | addrl;
		break;
	}
	case (JMS): {
		//unsigned char addrh = opa;
		// not used since there is only 1 rom right now
		unsigned char addrm = (cpu->rom[cpu->pc++]);
		unsigned char addrl = (cpu->rom[cpu->pc++]);

		cpu->stack[cpu->sp++] = cpu->pc;
		cpu->pc = (addrm << 4) | addrl;
		break;
	}
	case (INC): {
		unsigned char regp = opa >> 1;
		cpu->regp[regp]+=(1 + (opa & 1) * 15);
		break;
	}
	case (ISZ): {
		unsigned char addrh = (cpu->rom[cpu->pc++]);
		unsigned char addrl = (cpu->rom[cpu->pc++]);

		unsigned char regp = opa >> 1;
		if (cpu->regp[regp]+=(1 + (opa & 1) * 15))
			cpu->pc = (addrh << 4) | addrl;
		break;
	}
	case (ADD): {
		unsigned char regp = opa >> 1;
		unsigned char regval = (opa & 1) ? (cpu->regp[regp] & 15) : (cpu->regp[regp] & (15 << 4)) >> 4;
		cpu->accumulator += regval + (cpu->flags & 1);
		cpu->flags = (cpu->accumulator & 0xf0 > 0);
		cpu->accumulator &= 0xf0;
		break;
	}
	case (SUB): {
		unsigned char regp = opa >> 1;
		unsigned char regval = (opa & 1) ? (cpu->regp[regp] & 15) : (cpu->regp[regp] & (15 << 4)) >> 4;
		cpu->accumulator -= regval + (cpu->flags & 1);
		cpu->flags = (cpu->accumulator & 0xf0 == 0);
		cpu->accumulator &= 0xf0;
		break;
	}
	case (LD): {
		unsigned char regp = opa >> 1;
		unsigned char regval = (opa & 1) ? (cpu->regp[regp] & 15) : (cpu->regp[regp] & (15 << 4)) >> 4;
		cpu->accumulator = regval;
		break;
	}
	case (XCH): {
		unsigned char regp = opa >> 1;
		unsigned char regval = (opa & 1) ? (cpu->regp[regp] & 15) : (cpu->regp[regp] & (15 << 4)) >> 4;

		if (opa & 1){
			cpu->regp[regp] &= cpu->regp[regp] & 0xf0 | cpu->accumulator;
		} else {
			cpu->regp[regp] &= cpu->regp[regp] & 0x0f | (cpu->accumulator << 4);
		}
		cpu->accumulator = regval;
		break;
	}
	case (BBL): {
		cpu->pc = cpu->stack[cpu->sp--];
		cpu->accumulator = opa;
		break;
	}
	case (LDM): {
		cpu->accumulator = opa;
		break;
	}
	}
}