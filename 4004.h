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

	struct mem_4001* mem;
};

struct cpu_4004 * create_cpu();
void reset_cpu(struct cpu_4004*);
void destroy_cpu(struct cpu_4004*);
