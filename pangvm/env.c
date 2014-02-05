#include "env.h"
#include "bytecode.h"
#include "log.h"
#include "dbg.h"

#include <malloc.h>
#include <string.h>



static void execute_op(struct env*, const uint8 *ops);



uint
arglen(uint8 op) 
{
	uint8 head = op & OP_MASK_HEAD;
	uint8 body, arg;
	opcode_split(op, &body, &arg);

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
		} else if ((op & OP_MASK_STACKMEM) == OP_MOV_LTRL) {
			return 4;
		}
	} else if (head == OP_HEAD_FUNC) {
		return 0;
	} else if (head == OP_HEAD_ARIT_CMP) {
		if (arg == OP_ARG_ARIT_CMP_REG) {
			return 1;
		}
		return 4;
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
		*arg = op & OP_MASK_ARIT_CMP_ARG;
	} else {
		return -1;
	}

	return 0;
}


void 
execute(const uint8 *opcodes, uint len) 
{
	struct env *env = env_create();
	env->reg->r_ip = *(uint*)opcodes;

	printf("Starting at byte %u\n", env->reg->r_ip);

	while (env->reg->r_ip < len) {
		execute_op(env, opcodes);
	}

	env_destroy(env);
}

static void
execute_op(struct env *env, const uint8 *ops)
{
	dbg_registers(env->reg);
	dbg_opcode(ops, env->reg->r_ip);

	uint8 op;
	uint8 body;
	uint8 arg;
	const void *exarg = NULL;

	op = ops[env->reg->r_ip++];

	exarg = &ops[env->reg->r_ip];
	env->reg->r_ip += arglen(op);

	opcode_split(op, &body, &arg);

	switch (body) {
		case OP_PUSH:	op_push(env, op, exarg);	break;
		case OP_POP:	op_pop(env, op, exarg); 	break;
		case OP_MOV:	op_mov(env, op, exarg);		break;
		case OP_MOV_LTRL: op_mov_l(env, op, exarg);	break;

		case OP_CALL:	op_call(env, op, exarg);	break;
		case OP_RET:	op_ret(env, op, exarg);		break;

		case OP_ADD:	op_add(env, op, exarg);		break;
		case OP_SUB:	op_sub(env, op, exarg);		break;
		case OP_MUL:	op_mul(env, op, exarg);		break;
		case OP_DIV:	op_div(env, op, exarg);		break;
		case OP_MOD:	op_mod(env, op, exarg);		break;

		case OP_XOR:	op_xor(env, op, exarg);		break;
		case OP_AND:	op_and(env, op, exarg);		break;
		case OP_CMP:	op_cmp(env, op, exarg);		break;

		case OP_JMP:	op_jmp(env, op, exarg);		break;
		case OP_JE:		op_je(env, op, exarg);		break;
		case OP_JNE:	op_jne(env, op, exarg);		break;
		case OP_JL:		op_jl(env, op, exarg);		break;
		case OP_JLE:	op_jle(env, op, exarg);		break;
		case OP_JG:		op_jg(env, op, exarg);		break;
		case OP_JGE:	op_jge(env, op, exarg);		break;
	}
}


struct regs*
regs_create() 
{
	struct regs *regs = (struct regs*)malloc(sizeof(struct regs));

	memset(regs, 0, sizeof(struct regs));

	return regs;
}

void
regs_destroy(struct regs *env) 
{
	free(env);
}



struct memory*
memory_create()
{
	struct memory *memory;
	memory = (struct memory*)malloc(sizeof(struct memory));

	memory->len = MEMORY_INITIAL_SIZE;
	memory->data = malloc(memory->len);

	return memory;
}

void 
memory_destroy(struct memory *memory)
{
	free(memory->data);
	free(memory);
}

void 
memory_expand(struct memory *memory) 
{
	memory->len *= 2;
	memory->data = realloc(memory->data, memory->len);
}



struct env*
env_create()
{
	struct env *env;

	env = (struct env*)malloc(sizeof(struct env));
	memset(env, 0, sizeof(struct env));

	env->reg = regs_create();
	env->mem = memory_create();

	return env;
}

void 
env_destroy(struct env *env) 
{
	regs_destroy(env->reg);
	memory_destroy(env->mem);
	free(env);
}

void
env_increase_sp(struct env *env, uint len)
{
	env->reg->r_sp += len;

	if (env->reg->r_sp >= env->mem->len) {
		memory_expand(env->mem);
	}
}

int*
env_get_reg(struct env *env, uint8 reg)
{
	switch (reg) {
		case REG_A:		return &env->reg->r_a;
		case REG_B:		return &env->reg->r_b;
		case REG_C:		return &env->reg->r_c;
		case REG_D: 	return &env->reg->r_d;

		case REG_BP: 	return &env->reg->r_bp;
		case REG_SP: 	return &env->reg->r_sp;

	}

	return NULL;
}

int*
env_get_int(struct env *env, uint addr)
{
	if (env->mem->len + 4 > addr) {
		return (int*)env->mem->data + addr;
	}

	char str[128];
	sprintf(str, "Attempting to read address %u", addr);
	panglog(LOG_CRITICAL, str);

	return NULL;
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
	int value = *env_get_reg(env, op & OP_MASK_STACKMEM_REG);

	// Grow the stack by 4
	env_increase_sp(env, sizeof(int));

	// Get the pointer to the highest DWord
	int *ptr = env_get_int(env, env->reg->r_sp - sizeof(int));
	*ptr = value;
}

FUNC_OPCODE(op_pop) 
{
	int *reg = env_get_reg(env, op & OP_MASK_STACKMEM_REG);

	// Get the address of the int at the top of the stack
	env->reg->r_sp -= sizeof(int);
	int *stack = env_get_int(env, env->reg->r_sp);

	*reg = *stack;
}

FUNC_OPCODE(op_mov) 
{
	int *reg;	// Internal register arg
	int *ext;	// External arg

	reg = env_get_reg(env, op & OP_MASK_STACKMEM_REG);

	if (op & OP_MASK_MOV_32BIT) {
		int arg = *(int*)exarg;
		ext = env_get_int(env, arg);
	} else {
		uint8 arg = ((uint8*)exarg)[0];
		ext = env_get_reg(env, arg);
	}

	if (op & OP_MASK_MOV_TO_REG) {
		*reg = *ext;
	} else {
		*ext = *reg;
	}
}

FUNC_OPCODE(op_mov_l)
{
	int *reg;
	int val = *(int*)exarg;

	reg = env_get_reg(env, op & OP_MASK_STACKMEM_REG);

	*reg = val;
}


FUNC_OPCODE(op_call) 
{

}

FUNC_OPCODE(op_ret) 
{

}


const int*
arit_cmp_get_ext(struct env *env, uint8 argtype, const void *arg)
{
	if (argtype == OP_ARG_ARIT_CMP_REG) {
		return env_get_reg(env, *(uint8*)arg);
	}

	if (argtype == OP_ARG_ARIT_CMP_MEM) {
		return env_get_int(env, *(int*)arg);
	}

	if (argtype == OP_ARG_ARIT_CMP_LITERAL) {
		return (int*)arg;
	}

	if (argtype == OP_ARG_ARIT_CMP_DATA) {
		/* TODO */
	}

	panglog(LOG_CRITICAL,	"Unable to resolve argument type "
							"for arit_cmp instruction");
	return NULL;
}

FUNC_OPCODE(op_add) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a += *ext;
	}
}

FUNC_OPCODE(op_sub) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a -= *ext;
	}
}

FUNC_OPCODE(op_mul) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a *= *ext;
	}
}

FUNC_OPCODE(op_div) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a /= *ext;
	}
}

FUNC_OPCODE(op_mod) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a %= *ext;
	}
}


FUNC_OPCODE(op_xor) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a ^= *ext;
	}
}

FUNC_OPCODE(op_and) 
{
	const int *ext = arit_cmp_get_ext(
		env, (op & OP_MASK_ARIT_CMP_ARG), exarg);
	if (ext) {
		env->reg->r_a &= *ext;
	}
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
