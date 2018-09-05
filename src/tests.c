#include <assert.h>
#include "tests.h"

#define _ASSERT(function, operation, expected) { \
    __ASSERT(function() operation expected);\
}

#define REGISTER(function) jump_table[i] = function;

#define __ASSERT(expr)							\
  ((void) sizeof ((expr) ? 1 : 0), __extension__ ({			\
      if (expr)								\
        ; /* empty */							\
      else								\
        __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION);	\
    }))

u8 i;
bool (*jump_table[32])(void);

bool __test_pbkdf2_hmac_sha1()
{
    char pass[] = "random_password";
    unsigned char* salt = "12345678";

    char *result = malloc(2 * PBKDF2_OUTPUT_SIZE);

    bool res = PBKDF2_HMAC_SHA_X(pass, sizeof(pass), salt, PKCS5_SALT_LEN, result);
    if (!res) {
        DEBUG_PRINT("%s\n", "test failed");
        return false;
    }

    printf("Hex result: %s\n", result);
    return true;
}

void __tests()
{
	REGISTER(__test_pbkdf2_hmac_sha1);

    _ASSERT(jump_table[0], CMP_NE, true);
}