#include <stdio.h>
#include <malloc.h>

#include "env.h"
#include "log.h"
#include "bytecode.h"

void
print_help()
{
	printf("Usage:\n\tpangvm [file]\n");
}

int 
main(int argc, char *argv[]) 
{
	if (argc != 2) {
		print_help();
		return 1;
	}

	FILE *file = fopen(argv[1], "rb");

	fseek(file, 0L, SEEK_END);
	uint size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	uint8 *opcode = malloc(size);
	fread(opcode, 1, size, file);

	execute(opcode, size);
	
	return 0;
}