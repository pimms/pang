#ifndef __PANG_DBG_H__
#define __PANG_DBG_H__

#include "pangvm.h"
#include "env.h"


/* Print debug information about the idx'th instruction
 */
void dbg_opcode(const uint8 *ops, uint idx);

/* Print debug information about the registers
 */
void dbg_registers(struct regs *regs);


#endif /* __PANG_DBG_H__ */