#ifndef __PANG_ENV_H__
#define __PANG_ENV_H__

#include "../pang.h"


/* Returns the number of bytes in the operation's
 * external argument. */
uint arglen(uint8 op);


struct regs 
{	
	// Instruction pointer 
	int r_ip;

	// Base pointer
	int r_bp;

	// Stack pointer
	int r_sp;

	// General purpose registers
	int r_a; 	
	int r_b;
	int r_c;
	int r_d;
};

void execute(uint8 *opcodes, uint len);


struct regs* regs_create();
void regs_destroy(struct regs* regs);


struct env 
{
	struct regs *reg;
};

struct env* env_create();
void env_destroy(struct env *env);

#endif /*__PANG_ENV_H__*/