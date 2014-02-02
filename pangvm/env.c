#include "env.h"
#include "bytecode.h"

#include <malloc.h>
#include <string.h>



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


void 
execute(uint8 *opcodes, uint len) 
{
	
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