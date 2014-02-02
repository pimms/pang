#ifndef __PANG_ENV_H__
#define __PANG_ENV_H__


struct env 
{
	int ip;
};


/* Execute an array of opcodes 
 */
void execute(void *opcodes, int len);


#endif __PANG_ENV_H__