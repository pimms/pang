#ifndef __PANG_BYTECODE_H__
#define __PANG_BYTECODE_H__


/* The two high order bits in a bytecode defines
 * it's type.
 *
 * 0x0 	STACKMEM
 * 0x8 	FUNC
 * 0xC 	ARIT_CMP
 */
#define OP_MASK_HEAD 		0xC0
#define OP_HEAD_STACKMEM 	0x0
#define OP_HEAD_FUNC 		0x8
#define OP_HEAD_ARIT_CMP 	0xC




/* Stack and memory altering operations
 * Range: 0x10 - 0x2F
 *
 * The low order nibble defines how the external
 * argument should be treated. 
 */
#define OP_MASK_STACKMEM 0xF0
#define OP_MASK_STACKMEM_ARG 0x0F

// If the MOV_TO_REG bit is true in the OP_MOV opcode,
// the external argument should be moved into the register.
// Otherwise, the register value is moved to the external arg.
#define OP_MASK_MOV_TO_REG 0x04

// If the MOV_32BIT-bit is true in the OP_MOV opcode,
// the external argument is 32 bit. Otherwise, it's 8.
#define OP_MASK_MOV_32BIT  0x08

#define OP_PUSH			0x10
#define OP_POP 			0x20
#define OP_MOV 			0x30




/* Function operations
 * Range: 0x80 - 0x8F
 *
 * No arguments are required for any operation.
 */
#define OP_MASK_FUNC 0xFF

#define OP_CALL 		0x81
#define OP_RET 			0x82





/* Arithmetic and comparison operations
 * Range: 0xC0 - 0xFF
 *
 * All operation require an external 
 * argument. The low order bit is 1 if the external
 * argument is an 8-bit value containing a register 
 * reference, or 0 if it is a 32-bit address reference.
 */
#define OP_MASK_ARIT_CMP 0xFE
#define OP_MASK_ARIT_CMP_32BIT 0x01

#define OP_ADD 			0xC2
#define OP_SUB 			0xC4
#define OP_MUL 			0xC6
#define OP_DIV 			0xC8
#define OP_MOD 			0xCA
#define OP_LS 			0xCC
#define OP_RS 			0xCE

#define OP_XOR			0xD2
#define OP_AND			0xD4
#define OP_CMP			0xD6

#define OP_JMP 			0xF2
#define OP_JE 			0xF4
#define OP_JNE			0xF6
#define OP_JL 			0xF8
#define OP_JLE 			0xFA
#define OP_JG 			0xFC
#define OP_JGE 			0xFE


#endif /* __PANG_BYTECODE_H__ */