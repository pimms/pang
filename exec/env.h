#ifndef __PANG_ENV_H__
#define __PANG_ENV_H__

#include "../pang.h"



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

struct regs_stack
{
	struct regs *cur;
	struct regs_stack *prev;
};


void execute(uint8 *opcodes, uint len);


struct regs* regs_create();
void regs_destroy(struct regs* regs);
struct regs* regs_copy(struct regs* orig);

struct regs_stack* regs_stack_create();
void regs_stack_destroy(struct regs_stack *stack);
struct regs_stack* regs_stack_push(struct regs_stack *stack);
struct regs_stack* regs_stack_pop(struct regs_stack *stack);

/* Returns the number of bytes in the operations
 * external argument. */
uint arglen(uint8 op);


#endif /*__PANG_ENV_H__*/