#ifndef __PANG_FILEIO_H__
#define __PANG_FILEIO_H__

#include <stdio.h>

/* Read at most "max" bytes from the file, discarding
 * the rest of the line.
 * Returns 0 if reading succeeded, -1 otherwise.
 */
int file_get_line(char *buf, int max, FILE *fp);


#endif /* __PANG_FILEIO_H__ */