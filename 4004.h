struct cpu_4004 {
	unsigned char accumulator;
	unsigned char cond;
	unsigned char regp[8];
	unsigned short pc;
	unsigned short stack[3];
};

struct cpu_4004 * create_cpu();
