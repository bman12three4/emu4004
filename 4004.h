#include "4001.h"
#include "4002.h"

struct rom_node {
	struct rom_4001* rom;
	int id;
	struct rom_node* next;
};

struct ram_node {
	struct ram_4002* ram;
	int id;
	struct ram_node* next;
};

struct cpu_4004 {
	unsigned char accumulator;
	unsigned char flags;
	unsigned char regp[8];
	unsigned short pc;
	unsigned short stack[3];
	unsigned char sp;

	struct rom_node* rom_list;
	struct ram_node* ram_list;

	unsigned char io_addr;
	unsigned char cm_ram;

	unsigned char test;
};

enum OPCODE {
	NOP,
	JCN,
	IDRP,
	INDR,
	JUN,
	JMS,
	INC,
	ISZ,
	ADD,
	SUB,
	LD,
	XCH,
	BBL,
	LDM,
	IO,
	ALU,
};

enum INDR_OPCODE {
	FIN,
	JIN,
};

enum IDRP_OPCODE {
	FIM,
	SRC,
};

enum IO_OPCODE {
	WRM,
	WMP,
	WRR,
	WPM,
	WR0,
	WR1,
	WR2,
	WR3,
	SBM,
	RDM,
	RDR,
	ADM,
	RD0,
	RD1,
	RD2,
	RD3,
};

enum ALU_OPCODE {
	CLB,
	CLC,
	IAC,
	CMC,
	CMA,
	RAL,
	RAR,
	TCC,
	DAC,
	TCS,
	STC,
	DAA,
	KBP,
	DCL,
};

struct cpu_4004 * create_cpu();
void reset_cpu(struct cpu_4004*);
void destroy_cpu(struct cpu_4004*);
void excecute_cpu(struct cpu_4004*);

int attach_rom(struct cpu_4004*, struct rom_4001*, int id);

#define get_reg(r) (r & 1) ? (cpu->regp[r >> 1] & 15) : ((cpu->regp[r >> 1] & (15 << 4)) >> 4);
#define set_reg(r, v) cpu->regp[r >> 1] = (r & 1) ? ((cpu->regp[r >> 1] & 0xf0) | v) : ((cpu->regp[r >> 1] & 0x0f) | (v << 4));
