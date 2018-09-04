#include "defines.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

char *__getenv(char *env_var)
{
	char *_env_str = getenv(env_var);

	if (_env_str == NULL) {
		DEBUG_PRINT("%d\n", errno);
		return NULL;
	}

	return _env_str;
}
