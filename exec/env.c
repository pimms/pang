#include "env.h"

#include <malloc.h>


static env*
env_create() 
{
	struct env *env = (env*)malloc(sizeof(env));
	return env;
}



void execute(void *opcodes, int len) 
{

}