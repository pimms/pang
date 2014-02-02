#ifndef __PANG_BYTECODE_H__
#define __PANG_BYTECODE_H__


/* Stack and memory altering operations
 * Range: 0x10 - 0x2F
 *
 * The low order nibble defines how the external
 * argument should be treated. 
 */
#define OP_PUSH			0x10
#define OP_POP 			0x20
#define OP_MOV 			0x30


/* Function operations
 * Range: 0x80 - 0x8F
 *
 * No arguments are required for any operation.
 */
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
#define OP_ARIT_CMP_MASK 0xFE
#define OP_ARIT_CMP_ARG_MASK 0x01

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