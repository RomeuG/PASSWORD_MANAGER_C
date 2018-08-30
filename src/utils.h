#ifndef __UTILS_H_
#define __UTILS_H_

#define DEBUG_PRINT(fmt, ...)											\
	do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__,		\
							__LINE__, __func__, __VA_ARGS__); } while (0)

#define CHECK(x, y, z, cmp, fmt, ...) do { \
	int retval = (x); \
	if (retval cmp y) { \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
		z; \
  	} \
} while (0)

#define CHECK_VAL(x, y, z, cmp, fmt, ...) do { \
	if ((x) cmp y) { \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
		z; \
  	} \
} while (0)

char *__getenv(char *env_var);

#endif