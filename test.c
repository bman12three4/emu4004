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
					printf("Error in LD: acc = %d, should be %d, regval = %d, should be %d\n", cpu->accumulator, val, regval, acc);
				}
				total_tests++;
			}
		}
	}
	printf("Done!\n\n");
}

int test_acc(struct cpu_4004* cpu)
{

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
	printf("Done!\n");
	printf("Total tests run: %d\n", total_tests);
}
