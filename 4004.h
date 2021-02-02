struct mem_4001 {
	unsigned char rom[256];
};

struct mem_4002 {

};

struct cpu_4004 {
	unsigned char accumulator;
	unsigned char cond;
	unsigned char regp[8];
	unsigned short pc;
	unsigned short stack[3];
	unsigned char sp;

	unsigned char rom[256];

	unsigned char test;
};

enum OPCODE {
	NOP,
	JCN,
	FIM,
	SRC,
	FIN,
	JIN,
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
	WRM,
	WMP,
	WRR,
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
