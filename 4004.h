struct mem_4001 {
	unsigned char rom[256];
};

struct mem_4002 {
	unsigned char ram[4][16];
};

struct cpu_4004 {
	unsigned char accumulator;
	unsigned char flags;
	unsigned char regp[8];
	unsigned short pc;
	unsigned short stack[3];
	unsigned char sp;

	unsigned char io_addr;
	unsigned char cm_ram;
	unsigned char ram_main[4][256];	// only 2 ram chips, could have up to 4
	unsigned char ram_stat[4][4];

	unsigned char rom[256];

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
	KPB,
	DCL,
};

struct cpu_4004 * create_cpu();
void reset_cpu(struct cpu_4004*);
void destroy_cpu(struct cpu_4004*);
void excecute_cpu(struct cpu_4004*);
