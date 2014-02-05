#include "dbg.h"
#include "bytecode.h"
#include "log.h"
#include "env.h"

#include <string.h>
#include <stdio.h>


static char gstr[160];

static char* get_op_push_info(uint8);
static char* get_op_pop_info(uint8);
static char* get_op_mov_info(uint8);

static const char*
get_op_info(uint8 op, uint8 full)
{
	switch (op) {
		case OP_PUSH:  	return get_op_push_info(full);
		case OP_POP:  	return get_op_pop_info(full);
		case OP_MOV:  	return get_op_mov_info(full);
		case OP_MOV_LTRL: return "OP_MOV_LTRL";

		case OP_CALL:  	return "OP_CALL";
		case OP_RET:  	return "OP_RET";
		case OP_ADD:  	return "OP_ADD";
		case OP_SUB:  	return "OP_SUB";
		case OP_MUL:  	return "OP_MUL";
		case OP_DIV:  	return "OP_DIV";
		case OP_MOD:  	return "OP_MOD";
		case OP_XOR:  	return "OP_XOR";
		case OP_AND:  	return "OP_AND";
		case OP_CMP:  	return "OP_CMP";
		case OP_JMP:  	return "OP_JMP";
		case OP_JE:  	return "OP_JE";	
		case OP_JNE:  	return "OP_JNE";
		case OP_JL:  	return "OP_JL";	
		case OP_JLE:  	return "OP_JLE";
		case OP_JG:  	return "OP_JG";	
		case OP_JGE:  	return "OP_JGE";
	}

	return "??";
}

static const char*
get_reg_name(uint8 reg) 
{
	switch (reg)
	{
		case REG_A : 	return "REG_A";
		case REG_B : 	return "REG_B";
		case REG_C : 	return "REG_C";
		case REG_D : 	return "REG_D";
		case REG_SP: 	return "REG_SP";
		case REG_BP: 	return "REG_BP";
	}

	return "??";
}

static char*
get_op_push_info(uint8 op)
{	
	op &= OP_MASK_STACKMEM_REG;
	sprintf(gstr, "OP_PUSH (%s)", get_reg_name(op));
	return gstr;
}

static char*
get_op_pop_info(uint8 op)
{	
	op &= OP_MASK_STACKMEM_REG;
	sprintf(gstr, "OP_POP (%s)", get_reg_name(op));
	return gstr;
}

static char*
get_op_mov_info(uint8 op)
{
	uint8 reg = op & OP_MASK_STACKMEM_REG;
	uint8 toreg = op & OP_MASK_MOV_TO_REG;
	uint8 bit32 = op & OP_MASK_MOV_TO_REG;

	sprintf(gstr, "OP_MOV (%s, ?) toreg:%d 32bit:%d", 
		get_reg_name(reg), toreg!=0, bit32!=0);

	return gstr;
}

void
dbg_opcode(const uint8 *ops, uint idx)
{
	uint8 op = ops[idx];

	uint8 body, arg;
	opcode_split(op, &body, &arg);

	panglog(LOG_DEBUG, get_op_info(body, op));
}



void 
dbg_registers(struct regs *r)
{
	char str[160];
	sprintf(str, "ra:%x rb:%x rc:%x rd:%x  ip:%x bp:%x sp:%x",
		r->r_a, r->r_b, r->r_c, r->r_d, r->r_ip, r->r_bp, r->r_sp);

	panglog(LOG_VERBOSE, str);
}