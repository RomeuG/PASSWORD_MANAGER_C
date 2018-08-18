#include "defines.h"

#include <stdlib.h>

char *__getenv(char *env_var) 
{ 
	char *_env_str = getenv(env_var);

	if (_env_str == NULL) {
		// FREE
		char *err = malloc(5);
		err = "NULL\0";
		return err;
	}

	return _env_str;
}