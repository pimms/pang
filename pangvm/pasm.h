#ifndef __PANG_ASM_H__
#define __PANG_ASM_H__

#include "pangvm.h"


// Linked list of intermediate instructions. One line of pasm
// code produces one pasm_instr struct. The structs are later
// used to generate the bytecode itself. 
struct pasm_instr 
{
	struct pasm_instr *next;
	uint8 oper;
	uint8 arg[4];
	uint arglen;
};

// Translate one line of pasm into a struct
struct pasm_instr* pasm_translate(const char *line);


uint8* pasm_compile(const char *file, int *opcodelen);


#endif /* __PANG_ASM_H__ */