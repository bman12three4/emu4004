#include <stdio.h>
#include "assembler.h"

static const char* opcodes[] = {
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

	fgets(line, 256, source);
	printf("%s\n", line);

}