#include "pasm.h"
#include "log.h"
#include "fileio.h"



uint8* 
pasm_compile(const char *file, int *opcodelen)
{
	FILE *fp = NULL;
	char buf[80];

	fp = fopen(file, "r");
	if (!fp) {
		panglog(LOG_CRITICAL, "Unable to open file");
		return NULL;
	}


	while (file_get_line(buf, 80, fp) == 0) {
		
	}

	opcodelen = 0;
	return 0;
}