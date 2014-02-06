#include "pasm.h"
#include "log.h"
#include "fileio.h"
#include "bytecode.h"
#include "env.h"

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>


// Upon error, set "error" to a non-zero value
// and put the error message in "emsg".
static struct {
	uint error;
	char emsg[256];
} compile_status;

void
pasm_error(uint err, const char *msg) 
{
	compile_status.error = err;
	if (msg) {
		strcpy(compile_status.emsg, msg);
	}
}



struct pasm_program*
pasm_program_create() 
{
	struct pasm_program *prog;
	prog = (struct pasm_program*)malloc(sizeof(struct pasm_program));
	memset(prog, 0, sizeof(struct pasm_program));
	return prog;
}

void 
pasm_program_destroy(struct pasm_program *program)
{
	while (program->head_instr) {
		struct pasm_instr *next = program->head_instr->next;

		free(program->head_instr);
		program->head_instr = next;
	}

	while (program->head_label) {
		struct pasm_label *next = program->head_label->next;

		free(program->head_label);
		program->head_label = next;
	}

	free(program);
}

void 
pasm_program_add_instr(	struct pasm_program *prog,
						struct pasm_instr *instr)
{
	if (!prog->head_instr) {
		prog->head_instr = instr;
		prog->tail_instr = instr;
	} else {
		prog->tail_instr->next = instr;
		prog->tail_instr = instr;
	}
}

void
pasm_program_add_label(	struct pasm_program *prog, 
						struct pasm_label *label)
{
	if (!prog->head_label) {
		prog->head_label = label;
		prog->tail_label = label;
	} else {
		prog->tail_label->next = label;
		prog->tail_label = label;
	}
}

void
pasm_program_add_line(struct pasm_program *prog, char *line)
{
	struct pasm_instr *instr;

	instr = pasm_translate_instr_line(line);
	pasm_program_add_instr(prog, instr);
}

uint8*
pasm_program_compile(struct pasm_program *prog, uint *len)
{
	struct pasm_instr *it = prog->head_instr;
	uint8 *opcode = NULL;

	// Calculate the required number of bytes.
	*len = 4;
	while (it) {
		it->offset = *len;
		*len += 1 + it->arglen;
		it = it->next;
	}

	// Store the instructions in the program area
	opcode = (uint8*)malloc(*len);
	memset(opcode, 0, *len);
	*(int*)opcode = 4; 	// Start execution at byte no. 5
	it = prog->head_instr;
	uint i = 4;

	while (it) {
		opcode[i++] = it->oper;

		if (it->arglen == 1) {
			opcode[i++] = it->arg8;
		} else if (it->arglen == 4) {
			*((uint*)(opcode+i)) = it->arg32;
			i += 4;
		}

		it = it->next;
	}

	return opcode;
}




uint8
pasm_get_op(const char *instr) 
{
	if (!strcmp(instr, "push")) 
		return OP_PUSH;
	if (!strcmp(instr, "pop"))
		return OP_POP;
	if (!strcmp(instr, "mov"))
		return OP_MOV;
	if (!strcmp(instr, "call"))
		return OP_CALL;
	if (!strcmp(instr, "ret"))
		return OP_RET;
	if (!strcmp(instr, "add"))
		return OP_ADD;
	if (!strcmp(instr, "sub"))
		return OP_SUB;
	if (!strcmp(instr, "mul"))
		return OP_MUL;
	if (!strcmp(instr, "div"))
		return OP_DIV;
	if (!strcmp(instr, "mod"))
		return OP_MOD;
	if (!strcmp(instr, "xor"))
		return OP_XOR;
	if (!strcmp(instr, "and"))
		return OP_AND;
	if (!strcmp(instr, "cmp"))
		return OP_CMP;
	if (!strcmp(instr, "jmp"))
		return OP_JMP;
	if (!strcmp(instr, "je"))
		return OP_JE;
	if (!strcmp(instr, "jne"))
		return OP_JNE;
	if (!strcmp(instr, "jl"))
		return OP_JL;
	if (!strcmp(instr, "jle"))
		return OP_JLE;
	if (!strcmp(instr, "jg"))
		return OP_JG;
	if (!strcmp(instr, "jge"))
		return OP_JGE;


	sprintf(compile_status.emsg, "Unkown instruction: %s", instr);
	pasm_error(1, NULL);

	return 0;
}

uint
pasm_get_arg_count(uint8 op)
{
	uint8 head = op & OP_MASK_HEAD;
	uint8 body, arg;
	opcode_split(op, &body, &arg);

	if (head == OP_HEAD_STACKMEM) {
		switch (body) {
			case OP_MOV:	return 2;
			default: 		return 1;
		}
	} else if (head == OP_HEAD_FUNC) {
		switch (body) {
			case OP_CALL:	return 1;
			default: 		return 0;
		}
	} else if (head == OP_HEAD_ARIT_CMP) {
		return 1;
	}

	char err[128];
	sprintf(err, "Unkown opcode head: 0x%X (0x%X)", head, op);
	panglog(LOG_CRITICAL, err);
	return 0;
}

uint 
pasm_get_arg_type(char *arg)
{
	if (!strcmp(arg, "ra") ||
		!strcmp(arg, "rb") ||
		!strcmp(arg, "rc") ||
		!strcmp(arg, "rd") ||
		!strcmp(arg, "rip") ||
		!strcmp(arg, "rbp") ||
		!strcmp(arg, "rsp")) {
		return PASM_ARG_REGISTER;
	}

	// Numerical literals are only available in 
	// base 10, and are prefixed with '$'
	if (arg[0] == '$' && strlen(arg) > 1) {
		for (int i=1; i<strlen(arg); i++) {
			if (arg[i] < '0' || arg[i] > '9') {
				return PASM_ARG_UNDEFINED;
			}
		}
		return PASM_ARG_LITERAL;
	}

	if ((strstr(arg, "(0x")==arg || strstr(arg, "(d0x")==arg) &&
		arg[strlen(arg)-1] == ')') {

		char *c = arg;
		while (*c != 'x') c++; c++; // fuck the police
		while (*c != ')') {
			if ((*c < '0' || *c > '9') &&
				(*c < 'A' || *c > 'F') &&
				(*c < 'a' || *c > 'f')) {
				return PASM_ARG_UNDEFINED;
			}
		}

		if (arg[1] == 'd') {
			return PASM_ARG_DATA;
		} else {
			return PASM_ARG_MEMORY;
		}
	}

	return PASM_ARG_UNDEFINED;
}

uint
pasm_get_reg_alias(char *reg)
{
	if (!strcmp(reg, "ra")) return REG_A;
	if (!strcmp(reg, "rb")) return REG_B;
	if (!strcmp(reg, "rc")) return REG_C;
	if (!strcmp(reg, "rd")) return REG_D;
	if (!strcmp(reg, "rip")) return REG_IP;
	if (!strcmp(reg, "rbp")) return REG_BP;
	if (!strcmp(reg, "rsp")) return REG_SP;

	panglog(LOG_CRITICAL, "Undefined register given");
	return 0;
}

uint
pasm_get_memory_addr(char *arg)
{
	// Addresses are on the form (0x...), iterate to the first x
	while (*arg != 'x')  arg++; arg++;

	// Shorten the string by one 
	arg[strlen(arg)-1] = 0;
	return atoi(arg);
}

int
pasm_get_literal(char *arg)
{
	// Literals are prefixed by a "$". 
	return atoi(arg+1);
}



struct pasm_instr*
pasm_translate_instr_line(char *line)
{
	struct pasm_line pline;
	char *tok;

	memset(pline.instr, 0, 16);
	memset(pline.arg0, 0, 16);
	memset(pline.arg1, 0, 16);

	tok = strtok(line, " ");
	if (!tok) {
		return NULL;
	}

	strcpy(pline.instr, tok);

	tok = strtok(NULL, " ");
	if (tok) strcpy(pline.arg0, tok);

	tok = strtok(NULL, " ");
	if (tok) strcpy(pline.arg1, tok);

	if (strtok(NULL, " ")) {
		pasm_error(1, "Unexpected token");
		return NULL;
	}

	return pasm_translate_pasm_line(&pline);
}

struct pasm_label*
pasm_translate_label_line(char *line)
{
	if (!isalpha(line[0]) || line[strlen(line)-1] != ':') {
		return NULL;
	}

	for (int i = 1; i < strlen(line)-1; ++i) {
		if (!isalnum(line[i])) {
			return NULL;
		}
	}

	struct pasm_label *label;
	label = (struct pasm_label*)malloc(sizeof(struct pasm_label));
	memset(label, 0, sizeof(struct pasm_label));
	return label;
}


struct pasm_instr*
pasm_translate_pasm_line(struct pasm_line *pline)
{
	struct pasm_instr *instr;
	uint num_args = 0;

	uint8 opcode = pasm_get_op(pline->instr);

	instr = (struct pasm_instr*)malloc(sizeof(struct pasm_instr));
	instr->next = NULL;
	instr->oper = opcode;

	if (strlen(pline->arg0)) num_args++;
	if (strlen(pline->arg1)) num_args++;
	if (num_args != pasm_get_arg_count(instr->oper)) {
		pasm_error(9, "Invalid number of arguments");
		free(instr);
		return NULL;
	}

	uint8 head = instr->oper & OP_MASK_HEAD;

	if (head == OP_HEAD_STACKMEM) {
		pasm_translate_stackmem(pline, instr);
	} else if (head == OP_HEAD_FUNC) {
		pasm_translate_function(pline, instr);
	} else if (head == OP_HEAD_ARIT_CMP) {
		pasm_translate_aritcmp(pline, instr);
	} else {
		free(instr);
		return NULL;
	}

	return instr;
}

void
pasm_translate_stackmem(struct pasm_line *pline, 
						struct pasm_instr *instr) 
{
	if (instr->oper == OP_MOV) {
		pasm_translate_stackmem_mov(pline, instr);
		return;
	}

	// Pushing and popping an only be done to and from registers
	uint type = pasm_get_arg_type(pline->arg0);
	if (type != PASM_ARG_REGISTER) {
		pasm_error(2, "Argument is not a register");
		return;
	} 

	// encode the argument register into the opcode
	uint ralias = pasm_get_reg_alias(pline->arg0);
	instr->oper |= ralias;
}

void
pasm_translate_stackmem_mov(struct pasm_line *pline, 
							struct pasm_instr *instr) 
{
	uint8 at0, at1;
	at0 = pasm_get_arg_type(pline->arg0);
	at1 = pasm_get_arg_type(pline->arg1);

	if (at0 == PASM_ARG_UNDEFINED || at1 == PASM_ARG_UNDEFINED) {
		pasm_error(3, "Invalid argument");
		return;
	}

	if (at0 != PASM_ARG_REGISTER && at1 != PASM_ARG_REGISTER) {
		pasm_error(4, "One operand in MOV must be a register");
		instr = NULL;
		return;
	}

	char *reg = (at0==PASM_ARG_REGISTER) ? pline->arg0 : pline->arg1;
	char *arg = (reg==pline->arg0) ? pline->arg1 : pline->arg0;

	// Encode the register into the opcode
	instr->oper |= pasm_get_reg_alias(reg);

	// If "reg" is the destination, flag the "mov_to_reg" bit 
	if (reg == pline->arg0) {
		instr->oper |= OP_MASK_MOV_TO_REG;
	}

	// Encode the external argument and the opcode if nessecary.
	// We know at this point that both args are valid.
	uint8 argt = pasm_get_arg_type(arg);
	if (argt == PASM_ARG_REGISTER) {
		instr->arglen = 1;
		instr->arg8 = pasm_get_reg_alias(arg);
	} else if (argt == PASM_ARG_MEMORY) {
		instr->arglen = 4;
		instr->arg32 = pasm_get_memory_addr(arg);
	} else if (argt == PASM_ARG_DATA) {
		instr->arglen = 4;
		instr->arg32 = pasm_get_memory_addr(arg) | OP_MASK_MOV_EXTARG32_DATA;
	} else if (argt == PASM_ARG_LITERAL) {
		instr->arglen = 4;
		instr->oper = OP_MOV_LTRL | (instr->oper & OP_MASK_STACKMEM_REG);
		instr->arg32 = pasm_get_literal(arg);
	} 
}

void
pasm_translate_function(struct pasm_line *pline, 
						struct pasm_instr *instr) 
{
	pasm_error(999, "function not yet supported");
}

void
pasm_translate_aritcmp(struct pasm_line *pline, 
					   struct pasm_instr *instr) 
{
	// Exactly one argument is expected for all arit_cmp 
	// instructions. 
	if (!strlen(pline->arg0) || strlen(pline->arg1)) {
		pasm_error(9, "Invalid number of arguments");
	}

	instr->oper = pasm_get_op(pline->instr);

	// Currently, all types of arguments are allowed in
	// arit_cmp instructions.
	uint atype = pasm_get_arg_type(pline->arg0);
	if (atype == PASM_ARG_UNDEFINED) {
		pasm_error(1, "Invalid argument");
	}

	// Encode the argument type into the instruction
	switch (atype) {
		case PASM_ARG_REGISTER:	
			instr->oper |= OP_ARG_ARIT_CMP_REG;
			instr->arglen = 1;
			instr->arg8 = pasm_get_reg_alias(pline->arg0);
			break;
		case PASM_ARG_MEMORY:
			instr->oper |= OP_ARG_ARIT_CMP_MEM;
			instr->arglen = 4;
			instr->arg32 = pasm_get_memory_addr(pline->arg0);
			break;
		case PASM_ARG_DATA:
			instr->oper |= OP_ARG_ARIT_CMP_DATA;
			instr->arglen = 4;
			instr->arg32 = pasm_get_memory_addr(pline->arg0);
			break;
		case PASM_ARG_LITERAL:
			instr->oper |= OP_ARG_ARIT_CMP_LITERAL;
			instr->arglen = 4;
			instr->arg32 = pasm_get_literal(pline->arg0);
			break;
		default:
			pasm_error(1, "Unhandled argument type");
	}
}



char* 
pasm_clean_line(char *str)
{
	while (*str == ' ' || *str == '\t') str++;

	char *end = str + strlen(str);

	// Ignore comments 
	for (int i=0; i<strlen(str); i++) {
		if (str[i] == ';' || str[i] == '#') {
			end = str + i - 1;

			if (end <= str) {
				*str = 0;
				return str;
			}
			break;
		}
	}

	// Ignore trailing whitespace
	while ((!*end || *end == ' ' || *end == '\t') 
		&& end > str) {
		end--;
	}
	*end = 0;

	// convert all whitespace to spaces
	for (char *c = str; c != end && *c; c++) {
		if (*c == '\t' || *c == '\n') {
			*c = ' ';
		}
	}

	return str;
}

uint8* 
pasm_compile(const char *file, uint *opcodelen)
{
	struct pasm_program *prog = pasm_program_create();
	FILE *fp = NULL;
	compile_status.error = 0;
	char buf[80];
	uint line_no = 1;
	*opcodelen = 0;

	fp = fopen(file, "r");
	if (!fp) {
		panglog(LOG_CRITICAL, "Unable to open file");
		return NULL;
	}

	while (file_get_line(buf, 80, fp) == 0) {
		char *line = pasm_clean_line(buf);
		if (strlen(line) == 0) {
			continue;
		}

		pasm_program_add_line(prog, line);

		// Handle compilation errors
		if (compile_status.error) {
			char err[128];
			sprintf(err, "Error at line %u:\n %s", 
					line_no, compile_status.emsg);
			panglog(LOG_CRITICAL, err);
			return NULL;
		}

		line_no++;
	}

	uint8 *opcode;
	opcode = pasm_program_compile(prog, opcodelen);
	panglog(LOG_VERBOSE, "Compilation complete");

	return opcode;
}
