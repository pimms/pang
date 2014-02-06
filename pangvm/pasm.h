#ifndef __PANG_ASM_H__
#define __PANG_ASM_H__

#include "pangvm.h"


#define PASM_ARG_REGISTER 	0
#define PASM_ARG_LITERAL 	1
#define PASM_ARG_MEMORY 	2
#define PASM_ARG_DATA 		3
#define PASM_ARG_UNDEFINED 	4
#define PASM_ARG_LABEL 		5


struct pasm_line 
{
	char instr[16];
	char arg0[16];
	char arg1[16];
};


// Linked list of labels. Labels first reference the following
// instruction. The offset of the label is then calculated, and
// instructions referencing the label (jmps) then use this offset.
struct pasm_label
{
	struct pasm_label *next;
	struct pasm_instr *instr;
	char name[32];
	uint mem_offset;
};


// Linked list of intermediate instructions. One line of pasm
// code produces one pasm_instr struct.
struct pasm_instr 
{
	struct pasm_instr *next;
	struct pasm_label *label;
	uint8 oper;
	uint8 arg8;
	uint arg32;
	uint arglen;
	uint offset;
};

struct pasm_program 
{
	struct pasm_instr *head_instr;
	struct pasm_instr *tail_instr;

	struct pasm_label *head_label;
	struct pasm_label *tail_label;

	// The label_queue pointer points to the last added label 
	// with no instruction. Upon adding an instruciton, all 
	// labels following label_queue will reference that instruction.
	struct pasm_label *label_queue;
};

struct pasm_program* pasm_program_create();
void pasm_program_destroy(struct pasm_program*);
void pasm_program_add_instr(struct pasm_program*, struct pasm_instr*);
void pasm_program_add_label(struct pasm_program*, struct pasm_label*);
void pasm_program_add_line(struct pasm_program*, char *line);
uint8* pasm_program_compile(struct pasm_program*, uint *len);


// Get the naked bytecode from the instruction
uint8 pasm_get_op(const char *instr);
uint pasm_get_arg_count(uint8 op);
uint pasm_get_arg_type(char *arg);
uint pasm_get_reg_alias(char *reg);
uint pasm_get_memory_addr(char *arg);
int pasm_get_literal(char *arg);


// Translate one line of pasm into an instruction. An intermediate
// type, pasm_line, is used before generating the instruction.
struct pasm_instr* pasm_translate_instr_line(char *line);

// Returns NULL if the line does not meet the label format spec.
// Labels must match the regex [a-zA-Z](a-zA-Z0-9)*: 
// 	(must start with a char, contain only letters and numbers, and
//   end with a colon). 
// Use this function to test wheter a line is a label or not. 
struct pasm_label* pasm_translate_label_line(char *line);

struct pasm_instr* pasm_translate_pasm_line(struct pasm_line *pline);
void pasm_translate_stackmem(struct pasm_line*, struct pasm_instr*);
void pasm_translate_stackmem_mov(struct pasm_line*, struct pasm_instr*);
void pasm_translate_function(struct pasm_line*, struct pasm_instr*);
void pasm_translate_aritcmp(struct pasm_line*, struct pasm_instr*);

// Remove comments and padding whitespace. All whitespace is 
// converted to " " (spaces).
char* pasm_clean_line(char *str);

// Join a set of instructions
uint8* pasm_join_instr(struct pasm_instr *first, uint *len);

uint8* pasm_compile(const char *file, uint *opcodelen);



#endif /* __PANG_ASM_H__ */