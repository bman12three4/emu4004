#include <stdio.h>
#include <string.h>

#include "assembler.h"

static const char* opcodes[NUM_OPS] = {
	"NOP",
	"JCN",
	"FIM",
	"SRC",
	"FIN",
	"JIN",
	"JUN",
	"JMS",
	"INC",
	"ISZ",
	"ADD",
	"SUB",
	"LD",
	"XCH",
	"BBL",
	"LDM",
	"WRM",
	"WMP",
	"SRR",
	"WR0",
	"WR1",
	"WR2",
	"WR3",
	"SBM",
	"RDM",
	"RDR",
	"ADM",
	"RD0",
	"RD1",
	"RD2",
	"RD3",
	"CLB",
	"CLC",
	"IAC",
	"CMC",
	"CMA",
	"RAL",
	"RAR",
	"TCC",
	"DAC",
	"TCS",
	"STC",
	"DAA",
	"KBP",
	"DCL",
};

int is_op(char* str)
{
	int i;
	for (i = 0; i < NUM_OPS; i++) {
		if (!strcmp(str, opcodes[i]))
			return 1;
	}
	return 0;
}

int handle_op(char* ops[256])
{
	return 0;
}

int handle_symbol(char* str)
{
	return 0;
}

int main(int argc, char *argv[])
{
	FILE* source;

	if (argc < 2){
		printf("Not enough arguments.\n");
		return -1;
	}

	printf("Using source file %s\n", argv[1]);
	
	source = fopen(argv[1], "r");
	if (!source) {
		printf("Bad argument.\n");
		return -1;
	}

	assemble(source);
}

int assemble(FILE* source)
{
	char line[256];
	char ops[4][256];

	FILE* output;

	int numargs;
	int i;

	output = fopen("out.mcs4", "w");


	while(fgets(line, 256, source)){
		if (line[0] == ';')
			continue;

		numargs = sscanf(line, "%s %s %s %s\n", ops[0], ops[1], ops[2], ops[3]);
		printf("Got : ");
		for (i = 0; i < numargs; i++) {
			if (ops[i][0] == ';')
				break;
			printf("%s\t", ops[i]);
			if (is_op(ops[0])) {
				handle_op(ops);
			} else {
				handle_symbol(ops[0]);
			}
			printf("op");
		}
		printf("\n");
	}

}