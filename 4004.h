#ifndef _4004_H
#define _4004_H

#define ROM 0
#define RAM 1

struct cpu_4004 {
	unsigned char accumulator;
	unsigned char flags;
	unsigned char regp[8];
	unsigned short pc;
	unsigned short stack[3];
	unsigned char sp;

	struct memory_node* rom_list;
	struct memory_node* ram_list;

	unsigned char io_addr;
	unsigned char cm_ram;

	unsigned char test;
};

struct memory_node {
	void (*write)(struct cpu_4004* cpu, void* memory, unsigned short addr, unsigned char val);
	unsigned char (*read)(struct cpu_4004* cpu, void* memory, unsigned short addr);
	void (*destroy)();
	void* device;
	int id;
	struct memory_node* next;
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

extern const char* opcode_names[];

struct cpu_4004 * create_cpu();
void reset_cpu(struct cpu_4004*);
void destroy_cpu(struct cpu_4004*);
void excecute_cpu(struct cpu_4004*);

int attach_rom(struct cpu_4004*, struct memory_node*, int id);

#define get_reg(r) (r & 1) ? (cpu->regp[r >> 1] & 15) : ((cpu->regp[r >> 1] & (15 << 4)) >> 4)
#define set_reg(r, v) cpu->regp[r >> 1] = (r & 1) ? ((cpu->regp[r >> 1] & 0xf0) | v) : ((cpu->regp[r >> 1] & 0x0f) | (v << 4))

/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

//Regular underline text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"

//Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

//High intensty background 
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

//High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

//Bold high intensity text
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"

//Reset
#define reset "\e[0m"

#endif