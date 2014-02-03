#include "fileio.h"
#include <string.h>


int
file_get_line(char *buf, int max, FILE *fp)
{
	if (!fp || !buf || max < 1) {
		return -1;
	}

	fgets(buf, max, fp);
	
	if (strlen(buf) == 0) {
		return -1;
	}

	if (strlen(buf) == max-1) {
		// Discard the rest of the line
		char disc[80];
		fgets(disc, 80, fp);

		while (strlen(disc) == 79) {
			fgets(disc, 80, fp);
		}
	}

	return 0;
}