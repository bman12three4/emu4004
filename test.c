#include <stdio.h>
#include "4004.h"
#include "test.h"

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
		}
	}
	printf("Done!\n\n");
}

int test_index_to_acc(struct cpu_4004* cpu)
{

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
	printf("Done!\n");
}
