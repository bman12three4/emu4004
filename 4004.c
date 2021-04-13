#include <stdlib.h>
#include <stdio.h>
#include "4004.h"

unsigned char read_rom(struct cpu_4004* cpu, unsigned short addr);
unsigned char read_ram(struct cpu_4004* cpu, unsigned short addr);
void write_ram(struct cpu_4004* cpu, unsigned short addr, unsigned char val);

struct cpu_4004 * create_cpu()
{
	struct cpu_4004 *cpu = malloc(sizeof(struct cpu_4004));
	cpu->rom_list = NULL;
	cpu->ram_list = NULL;
	return cpu;
}

void reset_cpu(struct cpu_4004* cpu)
{

}

void destroy_cpu(struct cpu_4004* cpu)
{
	free(cpu);
}

int attach_rom(struct cpu_4004* cpu, struct rom_4001* rom, int id)
{
	struct rom_node* node = malloc(sizeof(struct rom_node));

	node->rom = rom;
	node->id = id;

	node->next = NULL;

	if (!cpu->rom_list) {
		cpu->rom_list = node;
	} else {
		cpu->rom_list->next = node;
	}

}

int attach_ram(struct cpu_4004* cpu, struct ram_4002* ram, int id)
{
	struct ram_node* node = malloc(sizeof(struct ram_node));

	node->ram = ram;
	node->id = id;

	node->next = NULL;

	if (!cpu->ram_list) {
		cpu->ram_list = node;
	} else {
		cpu->ram_list->next = node;
	}

}

unsigned char read_rom(struct cpu_4004* cpu, unsigned short addr)
{
	int id = (addr & 0xf00) >> 8;

	struct rom_node* node = cpu->rom_list;
	while (node->id != id) {
		node = node->next;
	}

	return node->rom->data[addr&0xff];
}

unsigned char read_ram(struct cpu_4004* cpu, unsigned short addr)
{
	int id = cpu->cm_ram << 2 | (addr & 0xf0) >> 4;

	struct ram_node* node = cpu->ram_list;
	while (node->id != id) {
		node = node->next;
	}

	return node->ram->data[(addr & 0x30) >> 4][addr & 0xf];
}

void write_ram(struct cpu_4004* cpu, unsigned short addr, unsigned char val)
{
	int id = cpu->cm_ram << 2 | (addr & 0xf0) >> 4;

	struct ram_node* node = cpu->ram_list;
	while (node->id != id) {
		node = node->next;
	}

	node->ram->data[(addr & 0x30) >> 4][addr & 0xf] = val;
}

unsigned char read_ram_status(struct cpu_4004* cpu, unsigned short addr, int n)
{
	int id = cpu->cm_ram << 2 | (addr & 0xf0) >> 4;

	struct ram_node* node = cpu->ram_list;
	while (node->id != id) {
		node = node->next;
	}

	return node->ram->data[(addr & 0x30) >> 4][1 + n + addr & 0xf];
}

void write_ram_status(struct cpu_4004* cpu, unsigned short addr, int n, unsigned char val)
{
	int id = cpu->cm_ram << 2 | (addr & 0xf0) >> 4;

	struct ram_node* node = cpu->ram_list;
	while (node->id != id) {
		node = node->next;
	}

	node->ram->data[(addr & 0x30) >> 4][1 + n + addr & 0xf] = val;
}

void excecute_cpu(struct cpu_4004* cpu)
{
	unsigned char opr = read_rom(cpu, cpu->pc++);
	unsigned char opa = read_rom(cpu, cpu->pc++);

	switch (opr){
	case NOP: {
		break;
	}
	case JCN: {
		int jump = (((~opa)&8) && (((cpu->accumulator == 0) && opa&4) || ((cpu->flags == 1) && opa&2) || (((~cpu->test)&1) && opa&1)) ||
		(opa&8) && (((cpu->accumulator != 0) && opa&4) || ((cpu->flags == 0) && opa&2) || ((cpu->test&1) && opa&1)));
		if (jump){
			unsigned char addrh = read_rom(cpu, cpu->pc++);
			unsigned char addrl = read_rom(cpu, cpu->pc++);
			cpu->pc = (addrh << 4) | addrl;
		}
		break;
	}
	case (IDRP): {
		switch (opa & 1) {
		case (FIM): {
			unsigned char reg = opa >> 1;
			unsigned char datah = read_rom(cpu, cpu->pc++);
			unsigned char datal = read_rom(cpu, cpu->pc++);

			cpu->regp[reg] = (datah << 4) | (datal & 0xf);
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
			cpu->regp[opa >> 1] = read_rom(cpu, cpu->regp[0]);
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
		unsigned char addrm = read_rom(cpu, cpu->pc++);
		unsigned char addrl = read_rom(cpu, cpu->pc++);

		cpu->pc = (addrm << 4) | addrl;
		break;
	}
	case (JMS): {
		//unsigned char addrh = opa;
		// not used since there is only 1 rom right now
		unsigned char addrm = read_rom(cpu, cpu->pc++);
		unsigned char addrl = read_rom(cpu, cpu->pc++);

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
		unsigned char addrh = read_rom(cpu, cpu->pc++);
		unsigned char addrl = read_rom(cpu, cpu->pc++);

		int val = get_reg(opa);
		val = (val+1) & 0xf;
		set_reg(opa, val);
		if (val)
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
		--cpu->sp;
		if (cpu->sp > 2)
			cpu->sp = 2;
		cpu->pc = cpu->stack[cpu->sp];
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
			write_ram(cpu, cpu->io_addr, cpu->accumulator);
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
			write_ram_status(cpu, cpu->io_addr, 0, cpu->accumulator);
			break;
		}
		case (WR1): {
			write_ram_status(cpu, cpu->io_addr, 1, cpu->accumulator);
			break;
		}
		case (WR2): {
			write_ram_status(cpu, cpu->io_addr, 2, cpu->accumulator);
			break;
		}
		case (WR3): {
			write_ram_status(cpu, cpu->io_addr, 3, cpu->accumulator);
			break;
		}
		case (SBM): {
			unsigned char ramval = read_ram(cpu, cpu->io_addr);
			cpu->accumulator -= ramval + (cpu->flags & 1);
			cpu->flags = (cpu->accumulator & 0xf0 == 0);
			cpu->accumulator &= 0xf0;
			break;
		}
		case (RDM): {
			cpu->accumulator = read_ram(cpu, cpu->io_addr);
			break;
		}
		case (RDR): {
			printf("RDR: Instruction not supported.\n");
			break;
		}
		case (ADM): {
			unsigned char ramval = read_ram(cpu, cpu->io_addr);
			cpu->accumulator += ramval + (cpu->flags & 1);
			cpu->flags = (cpu->accumulator & 0xf0 > 0);
			cpu->accumulator &= 0xf0;
			break;
		}
		case (RD0): {
			cpu->accumulator = read_ram_status(cpu, cpu->io_addr, 0);
			break;
		}
		case (RD1): {
			cpu->accumulator = read_ram_status(cpu, cpu->io_addr, 1);
			break;
		}
		case (RD2): {
			cpu->accumulator = read_ram_status(cpu, cpu->io_addr, 2);
			break;
		}
		case (RD3): {
			cpu->accumulator = read_ram_status(cpu, cpu->io_addr, 3);
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
			cpu->flags = (cpu->accumulator > 15);
			cpu->accumulator &= 0xf;
			break;
		}
		case (RAR): {
			cpu->accumulator |= cpu->flags << 4;
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
			cpu->accumulator = (cpu->accumulator-1) & 0xf;
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
			break;
		}
		case (KBP): {
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