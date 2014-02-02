#ifndef __PANG_LOG_H__
#define __PANG_LOG_H__


#define LOG_CRITICAL 	0
#define LOG_DEBUG 		1
#define LOG_VERBOSE 	2


void set_log_level(unsigned int level);

void panglog(unsigned int level, char *msg);



#define COLOR_DEFAULT 	"\033[0m"
#define COLOR_BLACK  	"\033[0;30m"
#define COLOR_RED 		"\033[0;31m"
#define COLOR_GREEN  	"\033[0;32m"
#define COLOR_BROWN  	"\033[0;33m"
#define COLOR_BLUE  	"\033[0;34m"
#define COLOR_MAGENTA  	"\033[0;35m"
#define COLOR_CYAN  	"\033[0;36m"
#define COLOR_LGRAY  	"\033[0;37m"

#endif /* __PANG_LOG_H__ */