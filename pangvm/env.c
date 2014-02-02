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

struct regs*
regs_copy(struct regs *orig)
{
	struct regs *cpy = regs_create();
	memcpy(cpy, orig, sizeof(struct regs));

	return cpy;
}

void
regs_destroy(struct regs *env) 
{
	free(env);
}




struct regs_stack*
regs_stack_create() 
{
	struct regs_stack *stack;

	stack = malloc(sizeof(struct regs_stack));
	memset(stack, 0, sizeof(struct regs_stack));

	stack->cur = regs_create();

	return stack;
}

void 
regs_stack_destroy(struct regs_stack *stack) 
{
	if (stack->cur) {
		regs_destroy(stack->cur);
	}

	free(stack);
}

struct regs_stack*
regs_stack_push(struct regs_stack *stack)
{
	struct regs_stack *cpy;

	cpy = regs_stack_create();
	cpy->prev = stack;

	return cpy;
}

struct regs_stack*
regs_stack_pop(struct regs_stack *stack) 
{
	struct regs_stack *prev = stack->prev;
	regs_stack_destroy(stack);
	return prev;
}



struct env*
env_create()
{
	struct env *env;

	env = malloc(sizeof(struct env));
	memset(env, 0, sizeof(struct env));

	env->stack = regs_stack_create();

	return env;
}

void 
env_destroy(struct env *env) 
{
	// Destroy all register containers
	while (env->stack) {
		env->stack = regs_stack_pop(env->stack);
	}

	free(env);
}