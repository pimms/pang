#ifndef __PANG_ENV_H__
#define __PANG_ENV_H__

#include "../pang.h"

struct env 
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


struct env* env_create();
void env_destroy(struct env* env);
struct env* env_copy(struct env* orig);

/* Returns the number of bytes in the operations
 * external argument. */
uint arglen(uint8 op);


#endif /*__PANG_ENV_H__*/