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
	cpu->io_addr = 0;
	cpu->cm_ram = 0;

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 256; j++){
			cpu->ram_main[i][j] = 0;
		}
		for (int j = 0; j < 4; j++){
			cpu->ram_stat[i][j] = 0;
		}
	}

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
	case (IDRP): {
		switch (opa & 1) {
		case (FIM): {
			unsigned char reg = opa >> 1;
			unsigned char addrh = (cpu->rom[cpu->pc++]);
			unsigned char addrl = (cpu->rom[cpu->pc++]);

			cpu->regp[reg] = cpu->rom[(addrh << 4) | addrl];
			
			break;
		}
		case (SRC): {
			cpu->io_addr = cpu->regp[opa >> 1];
			break;
		}
		}
		break;
	}
	case (INDR): {
		switch (opa & 1){
		case FIN : {
			cpu->regp[opa >> 1] = cpu->rom[cpu->regp[0]];
			break;
		}
		case JIN : {
			cpu->pc = cpu->regp[opa >> 1];
			break;
		}
		}
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
		if (cpu->sp > 2)
			cpu->sp = 0;
		cpu->pc = (addrm << 4) | addrl;
		break;
	}
	case (INC): {
		unsigned char val = get_reg(opa);
		val++;
		set_reg(opa, val);
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
		unsigned char regval = get_reg(opa);
		cpu->accumulator += regval + (cpu->flags & 1);
		cpu->flags = ((cpu->accumulator & 0xf0) > 0);
		cpu->accumulator &= 0xf;
		break;
	}
	case (SUB): {
		unsigned char regval = get_reg(opa);
		cpu->accumulator -= regval + (cpu->flags & 1);
		cpu->flags = (cpu->accumulator > 16);
		cpu->accumulator &= 0xf;
		break;
	}
	case (LD): {
		unsigned char regval = get_reg(opa);
		cpu->accumulator = regval;
		break;
	}
	case (XCH): {
		unsigned char regval = get_reg(opa);
		set_reg(opa, cpu->accumulator);
		cpu->accumulator = regval;
		break;
	}
	case (BBL): {
		cpu->pc = cpu->stack[cpu->sp--];
		if (cpu->sp > 2)
			cpu->sp = 2;
		cpu->accumulator = opa;
		break;
	}
	case (LDM): {
		cpu->accumulator = opa;
		break;
	}
	case (IO): {
		switch (opa){
		case (WRM): {
			cpu->ram_main[cpu->cm_ram][cpu->io_addr] = cpu->accumulator;
			break;
		}
		case (WMP): {
			printf("WMP: Instruction not supported.\n");
			break;
		}
		case (WRR): {
			printf("WRR: Instruction not supported.\n");
			break;
		}
		case (WPM): {
			printf("WPM: Instruction not supported.\n");
			break;
		}
		case (WR0): {
			cpu->ram_stat[cpu->cm_ram][0] = cpu->accumulator;
			break;
		}
		case (WR1): {
			cpu->ram_stat[cpu->cm_ram][1] = cpu->accumulator;
			break;
		}
		case (WR2): {
			cpu->ram_stat[cpu->cm_ram][2] = cpu->accumulator;
			break;
		}
		case (WR3): {
			cpu->ram_stat[cpu->cm_ram][3] = cpu->accumulator;
			break;
		}
		case (SBM): {
			unsigned char ramval = cpu->ram_main[cpu->cm_ram][cpu->io_addr];
			cpu->accumulator -= ramval + (cpu->flags & 1);
			cpu->flags = (cpu->accumulator & 0xf0 == 0);
			cpu->accumulator &= 0xf0;
			break;
		}
		case (RDM): {
			cpu->accumulator = cpu->ram_main[cpu->cm_ram][cpu->io_addr];
			break;
		}
		case (RDR): {
			printf("RDR: Instruction not supported.\n");
			break;
		}
		case (ADM): {
			unsigned char ramval = cpu->ram_main[cpu->cm_ram][cpu->io_addr];
			cpu->accumulator += ramval + (cpu->flags & 1);
			cpu->flags = (cpu->accumulator & 0xf0 > 0);
			cpu->accumulator &= 0xf0;
			break;
		}
		case (RD0): {
			cpu->accumulator = cpu->ram_stat[cpu->cm_ram][0];
			break;
		}
		case (RD1): {
			cpu->accumulator = cpu->ram_stat[cpu->cm_ram][1];
			break;
		}
		case (RD2): {
			cpu->accumulator = cpu->ram_stat[cpu->cm_ram][2];
			break;
		}
		case (RD3): {
			cpu->accumulator = cpu->ram_stat[cpu->cm_ram][3];
			break;
		}
		}
		break;
	}
	case (ALU): {
		switch (opa) {
		case (CLB): {
			cpu->accumulator = 0;
			cpu->flags = 0;
			break;
		}
		case (CLC): {
			cpu->flags = 0;
			break;
		}
		case (IAC): {
			cpu->accumulator++;
			if (cpu->accumulator & 0xf0){
				cpu->accumulator &=0xf;
				cpu->flags = 1;
			} else {
				cpu->flags = 0;
			}
			break;
		}
		case (CMC): {
			cpu->flags ^= 1;
			break;
		}
		case (CMA): {
			cpu->accumulator ^= 0xf;
			break;
		}
		case (RAL): {
			cpu->accumulator <<= 1;
			cpu->accumulator |= cpu->flags;
			cpu->accumulator &= 0xf;
			cpu->flags = (cpu->accumulator & 0xf0);
			break;
		}
		case (RAR): {
			cpu->accumulator |= cpu->flags << 5;
			cpu->flags = cpu->accumulator & 1;
			cpu->accumulator >>= 1;
			break;
		}
		case (TCC): {
			cpu->accumulator = cpu->flags;
			cpu->flags = 0;
			break;
		}
		case (DAC): {
			cpu->flags = (cpu->accumulator) ? 1 : 0;
			cpu->accumulator--;
			break;
		}
		case (TCS): {
			cpu->accumulator = (cpu->flags) ? 10 : 9;
			break;
		}
		case (STC): {
			cpu->flags = 1;
			break;
		}
		case (DAA): {
			if (cpu->accumulator > 9 || cpu->flags) {
				cpu->accumulator += 6;
				if (cpu->accumulator & 0xf0) {
					cpu->flags = 1;
				}
				cpu->accumulator &= 0xf;
			}
		}
		case (KPB): {
			switch (cpu->accumulator){
			case 0:
				break;
			case 1:
				cpu->accumulator = 1;
				break;
			case 2:
				cpu->accumulator = 2;
				break;
			case 4:
				cpu->accumulator = 3;
				break;
			case 8:
				cpu->accumulator = 4;
				break;
			default:
				cpu->accumulator = 0xf;
				break;
			break;
			}
		}
		case (DCL): {
			cpu->cm_ram = cpu->accumulator & 7;
			break;
		}
		}
		break;
	}
	}
}