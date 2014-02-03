#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "env.h"
#include "log.h"
#include "bytecode.h"
#include "pasm.h"

void
print_help()
{
	printf("Usage:\n\tpangvm [-pasm] <file>\n");
}

void
hex_dump(uint8 *opcode, uint len) 
{
	for (int i=0; i<len; i++) {
		printf("%X", opcode[i]);

		if (i%4 == 0 && i) {
			printf("\t");
		} else {
			printf(" ");
		}

		if (i%8 == 0 && i) {
			printf("\n");
		}
	}
}

int 
main(int argc, char *argv[]) 
{
	if (argc == 1) {
		print_help();
		return 1;
	}

	uint8 *opcode;
	uint len;

	if (argc == 2) {
		FILE *file = fopen(argv[1], "rb");

		fseek(file, 0L, SEEK_END);
		len = ftell(file);
		fseek(file, 0L, SEEK_SET);

		opcode = malloc(len);
		fread(opcode, 1, len, file);
	} else if (argc == 3 && !strcmp(argv[1], "-pasm")) {
		opcode = pasm_compile(argv[2], &len);
	} else {
		print_help();
	}

	if (!opcode || !len) {
		return 2;
	}

	hex_dump(opcode, len);

	//execute(opcode, len);
	
	return 0;
}