#include "env.h"
#include "bytecode.h"

#include <malloc.h>
#include <string.h>



static void execute_op(struct env*, uint8 *ops);



uint
arglen(uint8 op) 
{
	uint8 head = op & OP_MASK_HEAD;

	if (head == OP_HEAD_STACKMEM) {
		if ((op & OP_MASK_STACKMEM) == OP_PUSH) {
			return 0;
		} else if ((op & OP_MASK_STACKMEM) == OP_POP) {
			return 0;
		} else if ((op & OP_MASK_STACKMEM) == OP_MOV) {	
			if (op & OP_MASK_MOV_32BIT) {
				return 4;
			}
			return 1;
		}
	} else if (head == OP_HEAD_FUNC) {
		return 0;
	} else if (head == OP_HEAD_ARIT_CMP) {
		if (op & OP_MASK_ARIT_CMP_32BIT) {
			return 4;
		}
		return 1;
	}

	return 0;
}

int 
opcode_split(uint8 op, uint8 *body, uint8 *arg) 
{
	uint8 head = op & OP_MASK_HEAD;

	if (head == OP_HEAD_STACKMEM) {
		*body = op & OP_MASK_STACKMEM;
		*arg = op & OP_MASK_STACKMEM_ARG;
	} else if (head == OP_HEAD_FUNC) {
		*body = op & OP_MASK_FUNC;
		*arg = 0;
	} else if (head == OP_HEAD_ARIT_CMP) {
		*body = op & OP_MASK_ARIT_CMP;
		*arg = op & OP_MASK_ARIT_CMP_32BIT;
	} else {
		return -1;
	}

	return 0;
}


void 
execute(uint8 *opcodes, uint len) 
{
	struct env *env = env_create();

	while (env->reg->r_ip < len) {
		execute_op(env, opcodes);
	}

	env_destroy(env);
}

static void
execute_op(struct env *env, uint8 *ops)
{
	uint8 op;
	uint8 body;
	uint8 arg;

	op = ops[env->reg->r_ip++];
	opcode_split(op, &body, &arg);

	switch (body) {
		case OP_PUSH:		break;
		case OP_POP:		break;
		case OP_MOV:		break;

		case OP_CALL:		break;
		case OP_RET:		break;

		case OP_ADD:		break;
		case OP_SUB:		break;
		case OP_MUL:		break;
		case OP_DIV:		break;
		case OP_MOD:		break;
		case OP_LS:			break;
		case OP_RS:			break;

		case OP_XOR:		break;
		case OP_AND:		break;
		case OP_CMP:		break;

		case OP_JMP:		break;
		case OP_JE:			break;
		case OP_JNE:		break;
		case OP_JL:			break;
		case OP_JLE:		break;
		case OP_JG:			break;
		case OP_JGE:		break;
	}
}


struct regs*
regs_create() 
{
	struct regs *regs = malloc(sizeof(struct regs));

	memset(regs, 0, sizeof(struct regs));

	return regs;
}

void
regs_destroy(struct regs *env) 
{
	free(env);
}



struct env*
env_create()
{
	struct env *env;

	env = malloc(sizeof(struct env));
	memset(env, 0, sizeof(struct env));

	env->reg = regs_create();

	return env;
}

void 
env_destroy(struct env *env) 
{
	regs_destroy(env->reg);
	free(env);
}



/** OPCODE FUNCTIONS **/
/* FUNC_OPCODE functions have this signature:
 *		void f(struct env*, uint8 opcode, void* args)
 *
 * This WILL be fixed in the future. When I know that 
 * these arguments solve all my problems. 
 */
FUNC_OPCODE(op_push) 
{

}

FUNC_OPCODE(op_pop) 
{

}

FUNC_OPCODE(op_mov) 
{

}


FUNC_OPCODE(op_call) 
{

}

FUNC_OPCODE(op_ret) 
{

}


FUNC_OPCODE(op_add) 
{

}

FUNC_OPCODE(op_sub) 
{

}

FUNC_OPCODE(op_mul) 
{

}

FUNC_OPCODE(op_div) 
{

}

FUNC_OPCODE(op_mod) 
{

}

FUNC_OPCODE(op_ls) 
{

}

FUNC_OPCODE(op_rs) 
{

}


FUNC_OPCODE(op_xor) 
{

}

FUNC_OPCODE(op_and) 
{

}

FUNC_OPCODE(op_cmp) 
{

}


FUNC_OPCODE(op_jmp) 
{

}

FUNC_OPCODE(op_je) 
{

}

FUNC_OPCODE(op_jne) 
{

}

FUNC_OPCODE(op_jl) 
{

}

FUNC_OPCODE(op_jle) 
{

}

FUNC_OPCODE(op_jg) 
{

}

FUNC_OPCODE(op_jge) 
{

}
