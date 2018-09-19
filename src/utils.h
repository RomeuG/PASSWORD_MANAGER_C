#ifndef __UTILS_H_
#define __UTILS_H_

// DEBUG PRINT
#define DEBUG_PRINT(output, fmt, ...)									\
	do { if (DEBUG) fprintf(output, "%s:%d:%s(): " fmt, __FILE__,		\
							__LINE__, __func__, __VA_ARGS__);fflush(stdout);fflush(stderr); } while (0)

// check variable
#define CHECK(x, y, z, cmp, fmt, ...) do {		\
	int retval = (x); \
	if (retval cmp y) { \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
		z; \
  	} \
} while (0)

// check value
#define CHECK_VAL(x, y, z, cmp, fmt, ...) do {	\
	if ((x) cmp y) { \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
		z; \
  	} \
} while (0)

// macro free
#define _FREE(s) do { if (s) free (s); } while (0)

char *__getenv(char *env_var);

#endif
