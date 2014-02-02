#include "env.h"
#include "bytecode.h"

#include <malloc.h>
#include <string.h>


struct env*
env_create() 
{
	struct env *env = malloc(sizeof(struct env));

	memset(env, 0, sizeof(struct env));

	return env;
}

struct env*
env_copy(struct env *orig)
{
	struct env *cpy = env_create();
	memcpy(cpy, orig, sizeof(struct env));

	return cpy;
}

void
env_destroy(struct env *env) 
{
	free(env);
}



void 
execute(uint8 *opcodes, uint len) 
{
	struct env *env;

	env = env_create();

	while (opcodes[env->r_ip] != 0) 
	{
		uint8 op = opcodes[env->r_ip];
		uint len = arglen(op);
	} 

	env_destroy(env);
}


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
		return 4;
	}

	return 0;
}