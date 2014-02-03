#include "log.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>


static unsigned int g_log_level = LOG_VERBOSE;

void
set_log_level(unsigned int level) 
{
	if (level > LOG_VERBOSE) {
		level = LOG_VERBOSE;
	}

	g_log_level = level;
}

void 
panglog(unsigned int level, const char *msg) 
{
	if (level > g_log_level) {
		return;
	}

	char *lstr = malloc(strlen(msg) + 32);
	char *lvl;
	char *color;

	// Color the output
	switch (level) {
		case LOG_CRITICAL:
			color = COLOR_RED;
			lvl = "CRITICAL";
			break;
		case LOG_DEBUG:
			color = COLOR_BLUE;
			lvl = "DEBUG";
			break;
		case LOG_VERBOSE:
			color = COLOR_GREEN;
			lvl = "VERBOSE";
			break;
	}

	sprintf(lstr, "[%s%s%s] %s\n", color, lvl, COLOR_DEFAULT, msg);

	if (level == LOG_CRITICAL) {
		fputs(lstr, stderr);
	} else {
		fputs(lstr, stdout);
	}

	free(lstr);
}
