#include <stdio.h>
#include "4004.h"
#include "test.h"

int total_tests = 0;

int test_macros(struct cpu_4004* cpu)
{
	printf("Testing macros...\n");
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			set_reg(i, j);
			int val = get_reg(i);
			if (val != j){
				printf("Macro error\n");
			}
			total_tests++;
		}
	}
	printf("Done!\n\n");
}

int test_index(struct cpu_4004* cpu)
{
	printf("Testing index register instruction INC...\n");
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = INC;
			cpu->rom[cpu->pc+1] = i;

			set_reg(i, j);
			excecute_cpu(cpu);
			int val = get_reg(i);
			if (val != (j+1 & 0xf)){
				printf("Error in INC: r[%d] val[%d] returned %d\n", i, j, val);
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing index register instruction FIN...\n");
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 254; j++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = INDR;
			cpu->rom[cpu->pc+1] = i << 1 + FIN;
			cpu->regp[0] = j+2;
			cpu->rom[j+2] = 0x6e;
			excecute_cpu(cpu);

			if (cpu->regp[i] != 0x6e) {
				printf("Error in FIN\n");
			}

			int reg0 = get_reg(2*i);
			int reg1 = get_reg(2*i + 1);

			if (reg0 != 0x6 || reg1 != 0xe){
				printf("Macro error in FIN! Registers don't match pair.\n");
			}
			total_tests++;
		}
	}
	printf("Done!\n\n");
}

int test_index_to_acc(struct cpu_4004* cpu)
{
	printf("Testing index to accumulator instruction ADD...\n");

	for (int reg = 0; reg < 16; reg++){
		for (int acc = 0; acc < 16; acc++) {
			for (int val = 0; val < 16; val++){
				for (int carry = 0; carry < 2; carry++){
					reset_cpu(cpu);
					cpu->rom[cpu->pc] = ADD;
					cpu->rom[cpu->pc+1] = reg;
					cpu->accumulator = acc;
					set_reg(reg, val);
					cpu->flags = carry;
					excecute_cpu(cpu);

					if (((val + acc + carry) > 15 && cpu->flags == 0) || ((val + acc + carry) < 15 && cpu->flags == 1)) {
						printf("Error in ADD: carry flag incorrect\n");
					}

					if (cpu->accumulator != ((val + acc + carry) & 0xf)){
						printf("Error in ADD: acc = %d, should be %d\n", cpu->accumulator, ((val + acc + carry) & 0xf));
					}
					total_tests++;
				}
			}
		}
	}
	printf("Done!\n");

	printf("Testing index to accumulator instruction SUB...\n");
	for (int reg = 0; reg < 16; reg++){
		for (int acc = 0; acc < 16; acc++) {
			for (int val = 0; val < 16; val++){
				for (int carry = 0; carry < 2; carry++){
					reset_cpu(cpu);
					cpu->rom[cpu->pc] = SUB;
					cpu->rom[cpu->pc+1] = reg;
					cpu->accumulator = acc;
					set_reg(reg, val);
					cpu->flags = carry;
					excecute_cpu(cpu);

					if (((acc - val - carry) < 0 && cpu->flags == 0) || (((acc - val - carry) > 0 && cpu->flags == 1))){
						printf("Error in SUB: carry flag incorrect.\n");
					}

					if (cpu->accumulator != ((acc - val - carry) & 0xf)) {
						printf("Error in SUB: acc = %d, should be %d\n", cpu->accumulator, ((acc - val - carry) & 0xf));
					}
					total_tests++;
				}
			}
		}
	}
	printf("Done!\n");

	printf("Testing index to accumulator instruction LD...\n");
	for (int reg = 0; reg < 16; reg++){
		for (int val = 0; val < 16; val++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = LD;
			cpu->rom[cpu->pc+1] = reg;
			set_reg(reg, val);
			excecute_cpu(cpu);

			if (cpu->accumulator != val){
				printf("Error in LD: acc = %d, should be %d", cpu->accumulator, val);
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing index to accumulator instruction XCH...\n");
	for (int reg = 0; reg < 16; reg++){
		for (int val = 0; val < 16; val++){
			for (int acc = 0; acc < 16; acc++){
				reset_cpu(cpu);
				cpu->rom[cpu->pc] = XCH;
				cpu->rom[cpu->pc+1] = reg;
				cpu->accumulator = acc;
				set_reg(reg, val);
				excecute_cpu(cpu);

				int regval = get_reg(reg);
				if (cpu->accumulator != val || regval != acc){
					printf("Error in XCH: acc = %d, should be %d, regval = %d, should be %d\n", cpu->accumulator, val, regval, acc);
				}
				total_tests++;
			}
		}
	}
	printf("Done!\n\n");
}

int test_acc(struct cpu_4004* cpu)
{
	printf("Testing accumulator instruction CLB...\n");
	for (int acc = 0; acc < 16; acc++){
		for (int carry = 0; carry < 2; carry++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = ALU;
			cpu->rom[cpu->pc+1] = CLB;
			cpu->accumulator = acc;
			cpu->flags = carry;
			excecute_cpu(cpu);

			if (cpu->accumulator || cpu->flags) {
				printf("Error in CLB: acc = %d, should be 0, carry = %d, should be 0\n", cpu->accumulator, cpu->flags);
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing accumulator instruction CLC...\n");
	for (int carry = 0; carry < 2; carry++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = CLC;
		cpu->flags = carry;
		excecute_cpu(cpu);

		if (cpu->flags) {
			printf("Error in CLC: carry = %d, should be 0\n", cpu->flags);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction IAC...\n");
	for (int acc = 0; acc < 16; acc++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = IAC;
		cpu->accumulator = acc;
		excecute_cpu(cpu);

		if (cpu->accumulator != ((acc+1) & 0xf)){
			printf("Error in IAC: acc = %d, should be %d\n", cpu->accumulator, ((acc+1) & 0xf));
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction CMC...\n");
	for (int carry = 0; carry < 2; carry++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = CMC;
		cpu->flags = carry;
		excecute_cpu(cpu);

		if (cpu->flags == carry) {
			printf("Error in CMC: carry = %d, should be %d\n", cpu->flags, (~carry) & 1);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction CMA...\n");
	for (int acc = 0; acc < 16; acc++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = CMA;
		cpu->accumulator = acc;
		excecute_cpu(cpu);

		if (cpu->accumulator != ((~acc) & 0xf)){
			printf("Error in CMA: acc = %d, should be %d\n", cpu->accumulator, ((~acc) & 0xf));
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction RAL...\n");
	for (int acc = 0; acc < 16; acc++){
		for (int carry = 0; carry < 2; carry++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = ALU;
			cpu->rom[cpu->pc+1] = RAL;
			cpu->accumulator = acc;
			cpu->flags = carry;
			excecute_cpu(cpu);

			int new_carry = (acc >> 3) & 1;
			int new_acc = ((acc << 1) + carry) & 0xf;
			if (cpu->accumulator != new_acc || cpu->flags != new_carry) {
				printf("Error in RAL: acc = %d should be %d, carry = %d, should be %d\n", cpu->accumulator, new_acc, cpu->flags, new_carry);
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing accumulator instruction RAR...\n");
	for (int acc = 0; acc < 16; acc++){
		for (int carry = 0; carry < 2; carry++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = ALU;
			cpu->rom[cpu->pc+1] = RAR;
			cpu->accumulator = acc;
			cpu->flags = carry;
			excecute_cpu(cpu);

			int new_carry = acc & 1;
			int new_acc = ((acc >> 1) + (carry << 3)) & 0xf;
			if (cpu->accumulator != new_acc || cpu->flags != new_carry) {
				printf("Error in RAR: acc = %d should be %d, carry = %d, should be %d\n", cpu->accumulator, new_acc, cpu->flags, new_carry);
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing accumulator instruction TCC...\n");
	for (int carry = 0; carry < 2; carry++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = TCC;
		cpu->flags = carry;
		excecute_cpu(cpu);

		if (cpu->accumulator != carry) {
			printf("Error in TCC: acc = %d, should be %d\n", cpu->accumulator, carry);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction DAC...\n");
	for (int acc = 0; acc < 16; acc++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = DAC;
		cpu->accumulator = acc;
		excecute_cpu(cpu);

		if (cpu->accumulator != ((acc-1) & 0xf) || cpu->flags != (acc > 0)){
			printf("Error in DAC: acc = %d, should be %d, carry = %d, should be %d\n", cpu->accumulator, ((acc-1) & 0xf), cpu->flags, (acc > 1));
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction TCS...\n");
	for (int carry = 0; carry < 2; carry++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = TCS;
		cpu->flags = carry;
		excecute_cpu(cpu);

		if ((carry == 0 && cpu->accumulator != 9) || (carry == 1 && cpu->accumulator != 10)){
			printf("Error in TCS: acc = %d, should be %d\n", cpu->accumulator, (carry) ? 10 : 9);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction STC...\n");
	for (int carry = 0; carry < 2; carry++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = STC;
		cpu->flags = carry;
		excecute_cpu(cpu);

		if (cpu->flags != 1){
			printf("Error in STC: carry = %d, should be 1\n", cpu->flags);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing accumulator instruction DAA...\n");
	for (int acc = 0; acc < 16; acc++){
		for (int carry = 0; carry < 2; carry++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = ALU;
			cpu->rom[cpu->pc+1] = DAA;
			cpu->accumulator = acc;
			cpu->flags = carry;
			excecute_cpu(cpu);

			if (acc > 9 || carry > 0){
				if (cpu->accumulator != ((acc+6) & 0xf)){
					printf("Error in DAA: acc = %d, should be %d\n", cpu->accumulator, ((acc+6) & 0xf));
				}
				if (cpu->flags == 0 && acc+6 > 15){
					printf("Error in DAA: carry = %d, should be 1", cpu->flags);
				}
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing accumulator instruction KBP...\n");
	for (int acc = 0; acc < 16; acc++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = KBP;
		cpu->accumulator = acc;
		excecute_cpu(cpu);

		switch (acc){
		case (0):
			if (cpu->accumulator != 0){
				printf("Error in KBP: acc = %d, should be %d\n", cpu->accumulator, 0);
			}
			break;
		case (1):
			if (cpu->accumulator != 1){
				printf("Error in KBP: acc = %d, should be %d\n", cpu->accumulator, 1);
			}
			break;
		case (2):
			if (cpu->accumulator != 2){
				printf("Error in KBP: acc = %d, should be %d\n", cpu->accumulator, 2);
			}
			break;
		case (4):
			if (cpu->accumulator != 3){
				printf("Error in KBP: acc = %d, should be %d\n", cpu->accumulator,3);
			}
			break;
		case (8):
			if (cpu->accumulator != 4){
				printf("Error in KBP: acc = %d, should be %d\n", cpu->accumulator, 4);
			}
			break;
		default:
			if (cpu->accumulator != 0xf){
				printf("Error in KBP: acc = %d, should be %d\n", cpu->accumulator, 0xf);
			}
		}
		total_tests++;
	}
	printf("Done!\n\n");
}

int test_imm(struct cpu_4004* cpu)
{
	printf("Testing immediate instruction FIM...\n");
	for (int pair = 0; pair < 8; pair++){
		for (int val = 0; val < 256; val++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = IDRP;
			cpu->rom[cpu->pc+1] = (pair << 1) + FIM;
			cpu->rom[cpu->pc+2] = val >> 4;
			cpu->rom[cpu->pc+3] = val & 0xf;
			excecute_cpu(cpu);

			if (cpu->regp[pair] != val) {
				printf("FIM regp=%d, should be %d\n", cpu->regp[pair], val);

				printf("Error in FIM\n");
			}

			int reg0 = get_reg(2*pair);
			int reg1 = get_reg(2*pair + 1);

			if (reg0 != (val >> 4) || reg1 != (val & 0xf)){
				printf("Macro error in FIN! Registers don't match pair.\n");
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing immediate instruction LDM...\n");
	for (int acc = 0; acc < 16; acc++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = LDM;
		cpu->rom[cpu->pc+1] = acc;
		excecute_cpu(cpu);

		if (cpu->accumulator != acc){
			printf("Error in LDM: acc = %d, should be %d\n", cpu->accumulator, acc);
		}
		total_tests++;
	}
	printf("Done!\n\n");

}

int test_jcn(struct cpu_4004* cpu, int addr, int cond, int acc, int carry, int test){
	reset_cpu(cpu);
	cpu->rom[cpu->pc] = JCN;
	cpu->rom[cpu->pc+1] = cond;
	cpu->rom[cpu->pc+2] = addr >> 4;
	cpu->rom[cpu->pc+3] = addr & 0xf;
	cpu->accumulator = acc;
	cpu->flags = carry;
	cpu->test = test;
	excecute_cpu(cpu);

	int jump = (((~cond)&8) && (((acc == 0) && cond&4) || ((carry == 1) && cond&2) || (((~test)&1) && cond&1)) ||
		(cond&8) && (((acc != 0) && cond&4) || ((carry == 0) && cond&2) || ((test&1) && cond&1)));

	if (jump && (cpu->pc != addr)) {
		printf("Error in JCN: Failed to jump = %d cond=%d test=%d acc=%d flags=%d addr=%d\n", jump, cond, test, acc, carry, addr);
	} else if ((!jump && cpu->pc == addr) && addr != 2){
		printf("Error in JCN: incorrect jump = %d cond=%d test=%d acc=%d flags=%d addr=%d\n", jump, cond, test, acc, carry, addr);
	}

	total_tests++;
}

int test_toc(struct cpu_4004* cpu)
{
	printf("Testing transfer of control function JUN...\n");
	for (int addr = 0; addr < 1 << 8; addr++){
		reset_cpu(cpu);
		cpu->rom[cpu->pc] = JUN;
		cpu->rom[cpu->pc+1] = 0;
		cpu->rom[cpu->pc+2] = addr >> 4;
		cpu->rom[cpu->pc+3] = addr & 0xf;
		excecute_cpu(cpu);

		if (cpu->pc != addr){
			printf("Error in JUN: pc = %d, should be %d", cpu->pc, addr);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing transfer of control function JIN...\n");
	for (int regp = 0; regp < 8; regp++){
		for (int addr = 0; addr < 1 << 8; addr++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = INDR;
			cpu->rom[cpu->pc+1] = (regp << 1) + JIN;
			cpu->regp[regp] = addr;
			excecute_cpu(cpu);

			if (cpu->pc != addr){
				printf("Error in JIN: pc = %d, should be %d", cpu->pc, addr);
			}
			total_tests++;
		}
	}
	printf("Done!\n");

	printf("Testing transfer of control function JCN...\n");
	for (int cond = 0; cond < 1 << 4; cond++){
		for (int addr = 0; addr < 1 << 8; addr++){
			for (int acc = 0; acc < 1 << 4; acc++){
				for (int carry = 0; carry < 1 << 1; carry++){
					for (int test = 0; test < 1 << 1; test++){
						test_jcn(cpu, addr, cond, acc, carry, test);
					}
				}
			}
		}
	}
	printf("Done!\n");

	printf("Testing transfer of control function ISZ...\n");
	for (int reg = 0 ; reg < 16; reg++){
		for (int val = 0; val < 16; val++){
			for (int addr = 0; addr < 256; addr++){
				reset_cpu(cpu);
				cpu->rom[cpu->pc] = ISZ;
				cpu->rom[cpu->pc+1] = reg;
				cpu->rom[cpu->pc+2] = addr >> 4;
				cpu->rom[cpu->pc+3] = addr & 0xf;
				set_reg(reg, val)
				excecute_cpu(cpu);

				int regval = get_reg(reg);
				if (regval > 0 && cpu->pc != addr){
					printf("Error in ISZ: Failed to jump. val = %d regval=%d pc=%d addr=%d\n", val, regval, cpu->pc, addr);
				} else if(regval == 0 && (cpu->pc == addr && addr != 4)){
					printf("Error in ISZ: Incorrect jump. val = %d regval=%d pc=%d addr=%d\n", val, regval, cpu->pc, addr);
				}
				total_tests++;
			}
		}
	}
	printf("Done!\n\n");
}

int test_subr(struct cpu_4004* cpu)
{
	printf("Testing subroutine instruction JMS...\n");
	for (int addr = 0; addr < 256; addr++){
		for (int pc = 0; pc < 252; pc++){
			for (int sp = 0; sp < 3; sp++){
				reset_cpu(cpu);
				cpu->pc = pc;
				cpu->sp = sp;
				cpu->rom[cpu->pc] = JMS;
				cpu->rom[cpu->pc+1] = 0;
				cpu->rom[cpu->pc+2] = addr >> 4;
				cpu->rom[cpu->pc+3] = addr & 0xf;
				excecute_cpu(cpu);

				if (cpu->pc != addr){
					printf("Error in JMS: pc=%d, should be %d\n", cpu->pc, addr);
				}
				int newsp = (cpu->sp==0) ? 2 : cpu->sp-1;
				if (cpu->stack[newsp] != pc+4){
					printf("Error in JMS: sp=%d, stack[sp]=%d should be %d\n", newsp, cpu->stack[newsp], pc+4);
				}
				total_tests++;
			}
		}
	}
	printf("Done!\n");

	printf("Testing subroutine instruction BBL...\n");
	for (int addr = 0; addr < 256; addr++){
		for (int sp = 0; sp < 3; sp++){
			for (int val = 0; val < 16; val++){
				reset_cpu(cpu);
				cpu->sp = sp;
				cpu->rom[cpu->pc] = BBL;
				cpu->rom[cpu->pc+1] = val;
				cpu->stack[cpu->sp++] = addr;
				if (cpu->sp > 2)
					cpu->sp = 0;
				excecute_cpu(cpu);

				if (cpu->pc != addr){
					printf("Error in BBL: pc=%d, should be %d\n", cpu->pc, addr);
				}
				if (cpu->accumulator != val){
					printf("Error in BBL: acc=%d, should be %d\n", cpu->accumulator, val);
				}
				total_tests++;
			}
		}
	}
	printf("Done!\n\n");
}

int test_nop(struct cpu_4004* cpu)
{
	printf("Testing nop instruction NOP...\n");
	for (int addr = 0; addr < 256; addr++){
		for (int acc = 0; acc < 16; acc++){
			reset_cpu(cpu);
			cpu->pc = addr;
			cpu->accumulator = acc;
			cpu->rom[cpu->pc] = NOP;
			cpu->rom[cpu->pc+1] = 0;
			excecute_cpu(cpu);

			if (cpu->pc != addr+2){
				printf("Error in NOP: pc=%d, should be %d\n", cpu->pc, addr);
			}
			if (cpu->accumulator != acc){
				printf("Error in NOP: acc=%d, should be %d\n", cpu->accumulator, acc);
			}
			total_tests++;
		}
	}
	printf("Done!\n\n");
}

int test_memsel(struct cpu_4004* cpu)
{
	printf("Testing memsel instruction DCL...\n");

	for (int acc = 0; acc < 16; acc++){
		reset_cpu(cpu);
		cpu->accumulator = acc;
		cpu->rom[cpu->pc] = ALU;
		cpu->rom[cpu->pc+1] = DCL;
		excecute_cpu(cpu);

		int new_cm_ram = acc&7;
		if (cpu->cm_ram != new_cm_ram){
			printf("Error in DCL: cm_ram=%d, should be %d\n", cpu->cm_ram, new_cm_ram);
		}
		total_tests++;
	}
	printf("Done!\n");

	printf("Testing memsel instruction SRC...\n");
	for (int addr = 0; addr < 256; addr++){
		for (int rp = 0; rp < 8; rp++){
			reset_cpu(cpu);
			cpu->rom[cpu->pc] = IDRP;
			cpu->rom[cpu->pc+1] = (rp << 1)+ SRC;
			cpu->regp[rp] = addr;
			excecute_cpu(cpu);

			if (cpu->io_addr != addr){
				printf("Error in SRC: io_addr=%d, should be %d\n", cpu->io_addr, addr);
			}
			total_tests++;
		}
	}
	printf("Done!\n\n");
}

int test_io_ram(struct cpu_4004* cpu)
{
	
}

int test_all(struct cpu_4004* cpu)
{
	printf("Testing all instructions...\n");
	test_macros(cpu);
	test_index(cpu);
	test_index_to_acc(cpu);
	test_acc(cpu);
	test_imm(cpu);
	test_toc(cpu);
	test_subr(cpu);
	test_nop(cpu);
	test_memsel(cpu);
	printf("Done!\n");
	printf("Total tests run: %d\n", total_tests);
}
