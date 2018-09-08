#ifndef __TESTS_H
#define __TESTS_H

#include <assert.h>
#include <sys/time.h>

#include "utils.h"
#include "encryption.h"

#define TEST_NUMBER 1
#define TEST_FUNCTION_NAME_SIZE 64
#define OPERATOR_SIZE 3

#define MILLISECONDS 1000
#define MICROSECONDS 1000000

#define _ASSERT(function, operation, expected, name) \
do {                                                 \
	gettimeofday(&t_beginning, NULL);                \
    if (strcmp(operation, CMP_E_STR) == 0) {         \
        __ASSERT(function(), CMP_E, expected, name); \
    } else if (strcmp(operation, CMP_NE_STR) == 0) { \
        __ASSERT(function(), CMP_NE, expected, name);\
    } else if (strcmp(operation, CMP_G_STR) == 0) {  \
        __ASSERT(function(), CMP_G, expected, name); \
    } else if (strcmp(operation, CMP_GE_STR) == 0) { \
        __ASSERT(function(), CMP_GE, expected, name);\
    } else if (strcmp(operation, CMP_L_STR) == 0) {  \
        __ASSERT(function(), CMP_L, expected, name); \
    } else if (strcmp(operation, CMP_LE_STR) == 0) { \
        __ASSERT(function(), CMP_LE, expected, name);\
    }                                                \
} while (0)

#define REGISTER_TEST(_function, _operation, _expectation)      \
do {                                                            \
     jump_table[tests_registered].function = _function;         \
     strcpy(jump_table[tests_registered].operation, _operation);\
     strcpy(jump_table[tests_registered].name, #_function);     \
     jump_table[tests_registered].expectation = _expectation;   \
     tests_registered++;                                        \
} while (0)

#define ASSERT_ALL()                                                                                             \
do {                                                                                                             \
    for (int j = 0; j < TEST_NUMBER; j++) {                                                                      \
        _ASSERT(jump_table[j].function, jump_table[j].operation, jump_table[j].expectation, jump_table[j].name); \
    }                                                                                                            \
} while (0)

#define __ASSERT(function, operation, expected, name)							                                   \
do {                                                                                                               \
  ((void) sizeof ((function operation expected) ? 1 : 0), __extension__ ({			                               \
      if (function operation expected)    {                                                                        \
        successful_tests++;                                                                                        \
        gettimeofday(&t_ending, NULL);                                                                             \
        DEBUG_PRINT(stdout, "%s successful - %lu microseconds\n", name,                                            \
        		(t_ending.tv_sec*MICROSECONDS+t_ending.tv_usec) - (t_beginning.tv_sec*MICROSECONDS + t_beginning.tv_usec )); \
      } else {                                                                                                     \
        __assert_fail (name, __FILE__, __LINE__, __ASSERT_FUNCTION);                                               \
        }                                                                                                          \
    }));                                                                                                           \
} while (0)

struct __test_case {
	bool (*function)();
	char operation[OPERATOR_SIZE];
	char name[TEST_FUNCTION_NAME_SIZE];
	bool expectation;
};

// number of tests
extern u8 successful_tests;
extern u8 tests_registered;

// for time benchmarks
struct timeval t_beginning;
struct timeval t_ending;

// jump_table with the test functions
struct __test_case jump_table[TEST_NUMBER];

void __tests();

#endif
