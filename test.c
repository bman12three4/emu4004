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
	
}

int test_toc(struct cpu_4004* cpu)
{
	
}

int test_subr(struct cpu_4004* cpu)
{
	
}

int test_nop(struct cpu_4004* cpu)
{
	
}

int test_memsel(struct cpu_4004* cpu)
{
	
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
	printf("Done!\n");
	printf("Total tests run: %d\n", total_tests);
}
