#ifndef __TESTS_H
#define __TESTS_H

#include "utils.h"
#include "encryption.h"

#define TEST_NUMBER 2

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
     jump_table[tests_registered].function = _function;\
     strcpy(jump_table[tests_registered].operation, _operation);\
     jump_table[tests_registered].expectation = _expectation;\
     tests_registered++; \
}

#define ASSERT_ALL() {\
    for (int j = 0; j < TEST_NUMBER; j++) { \
        _ASSERT(jump_table[j].function, jump_table[j].operation, jump_table[j].expectation); \
    } \
}

#define __ASSERT(expr)							\
  ((void) sizeof ((expr) ? 1 : 0), __extension__ ({			\
      if (expr)    {                            \
        successful_tests++; /* TODO */                            \
        DEBUG_PRINT("%s successful\n", #expr);\
      } else {                            \
        __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION);    \
        } \
    }))


struct __test_case {
	bool (*function)();
	char operation[3];
	bool expectation;
};

void __tests();

#endif
