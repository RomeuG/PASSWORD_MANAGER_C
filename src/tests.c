#include <assert.h>
#include "tests.h"

#define _ASSERT(function, operation, expected) { \
    if (strcmp(operation, CMP_E_STR) == 0) {\
        __ASSERT(function() CMP_E expected);\
    } else if (strcmp(operation, CMP_NE_STR) == 0) { \
        __ASSERT(function() CMP_NE expected);\
    } else if (strcmp(operation, CMP_G_STR) == 0) { \
        __ASSERT(function() CMP_G expected);\
    } else if (strcmp(operation, CMP_GE_STR) == 0) { \
        __ASSERT(function() CMP_GE expected);\
    } else if (strcmp(operation, CMP_L_STR) == 0) { \
        __ASSERT(function() CMP_L expected);\
    } else if (strcmp(operation, CMP_LE_STR) == 0) { \
        __ASSERT(function() CMP_LE expected);\
    } \
}

#define REGISTER_TEST(_function, _operation, _expectation) { \
     jump_table[0].function = _function;\
     strcpy(jump_table[0].operation, _operation);\
     jump_table[0].expectation = _expectation;\
}

#define ASSERT_ALL() {\
    for (int j = 0; j < 1; j++) { \
        _ASSERT(jump_table[j].function, jump_table[j].operation, jump_table[j].expectation); \
    } \
}

#define __ASSERT(expr)							\
  ((void) sizeof ((expr) ? 1 : 0), __extension__ ({			\
      if (expr)								\
        i++; /* TODO */							\
      else								\
        __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION);	\
    }))


struct __test_case {
	bool (*function)();
	char operation[3];
	bool expectation;
};

u8 i = 0;
struct __test_case jump_table[32];

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
	REGISTER_TEST(__test_pbkdf2_hmac_sha1, CMP_E_STR, true);
    ASSERT_ALL();

    printf("%d out of %d tests complete!\n", i, i);
}