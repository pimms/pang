#ifndef __PANG_ENV_H__
#define __PANG_ENV_H__

#include "../pang.h"


/* Returns the number of bytes in the operation's
 * external argument. */
uint arglen(uint8 op);

/* Puts the instruction without arguments in "body", and the
 * arguments in "arg". 
 * Returns 0 on success, -1 on error. 
 */
int opcode_split(uint8 opcode, uint8 *body, uint8 *arg);


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


/* Opcode implementations are until they are known to function
 * properly declared via this macro.
 */
#define FUNC_OPCODE(_FUNCTION)			\
	void _FUNCTION(struct env *e, uint8 opcode, void *args)

FUNC_OPCODE(op_push);
FUNC_OPCODE(op_pop);
FUNC_OPCODE(op_mov);

FUNC_OPCODE(op_call);
FUNC_OPCODE(op_ret);

FUNC_OPCODE(op_add);
FUNC_OPCODE(op_sub);
FUNC_OPCODE(op_mul);
FUNC_OPCODE(op_div);
FUNC_OPCODE(op_mod);
FUNC_OPCODE(op_ls);
FUNC_OPCODE(op_rs);

FUNC_OPCODE(op_xor);
FUNC_OPCODE(op_and);
FUNC_OPCODE(op_cmp);

FUNC_OPCODE(op_jmp);
FUNC_OPCODE(op_je);
FUNC_OPCODE(op_jne);
FUNC_OPCODE(op_jl);
FUNC_OPCODE(op_jle);
FUNC_OPCODE(op_jg);
FUNC_OPCODE(op_jge);


#endif /*__PANG_ENV_H__*/