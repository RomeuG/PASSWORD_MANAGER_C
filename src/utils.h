#ifndef __UTILS_H_
#define __UTILS_H_

#define DEBUG_PRINT(fmt, ...)											\
	do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__,		\
							__LINE__, __func__, __VA_ARGS__); } while (0)

char *__getenv(char *env_var);

#endif