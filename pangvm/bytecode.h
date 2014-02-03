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
#define OP_HEAD_STACKMEM 	0x00
#define OP_HEAD_FUNC 		0x80
#define OP_HEAD_ARIT_CMP 	0xC0




/* Stack and memory altering operations
 * Range: 0x00 - 0x3F
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

// When moving data from the program (data) segment into a register,
// the 32 bit external argument has it's HO bit flagged. 
#define OP_MASK_MOV_EXTARG32_DATA 0x80000000

// The two low order bits are used to define the register
#define OP_MASK_STACKMEM_REG 0x03

#define OP_PUSH			0x00
#define OP_POP 			0x10
#define OP_MOV 			0x20
#define OP_MOV_LTRL 	0x30 	// Move literal into register




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
#define OP_MASK_ARIT_CMP 0xFC

// op & OP_MASK_ARIT_CMP_ARG can have four
// different values, defining the context of the external
// argument.
#define OP_MASK_ARIT_CMP_ARG 	0x03
#define OP_ARG_ARIT_CMP_REG 	0x00 	// An 8 bit register ref.
#define OP_ARG_ARIT_CMP_MEM 	0x01	// A 32 bit memory ptr
#define OP_ARG_ARIT_CMP_DATA 	0x02	// A 32 bit data ptr
#define OP_ARG_ARIT_CMP_LITERAL	0x03	// A 32 bit literal

#define OP_ADD 			0xC0
#define OP_SUB 			0xC4
#define OP_MUL 			0xC8
#define OP_DIV 			0xCC
#define OP_MOD 			0xD0

#define OP_XOR			0xD4
#define OP_AND			0xD8
#define OP_CMP			0xDC

#define OP_JMP 			0xE0
#define OP_JE 			0xE4
#define OP_JNE			0xE8
#define OP_JL 			0xEC
#define OP_JLE 			0xF0
#define OP_JG 			0xF4
#define OP_JGE 			0xF8



/* Registers may be stored in the two LO bits in
 * an operation, or stand by themselves in an 8 bit
 * external argument. Regardless, the same values are used.
 */
#define REG_A 			0x00
#define REG_B 			0x01
#define REG_C 			0x02
#define REG_D 			0x03
#define REG_IP 			0x04
#define REG_SP 			0x05
#define REG_BP 			0x06

/* The literal results of a comparison.
 */
#define CMP_RES_EQUAL 	0x01
#define CMP_RES_LESS 	0x02
#define CMP_RES_GREAT	0x04

#endif /* __PANG_BYTECODE_H__ */