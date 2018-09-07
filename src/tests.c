#include <assert.h>
#include "tests.h"

u8 successful_tests = 0;
u8 tests_registered = 0;
struct __test_case jump_table[TEST_NUMBER];

static bool __test_pbkdf2_hmac_sha1()
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

static bool _test2()
{
    return true;
}

void __tests()
{
	REGISTER_TEST(__test_pbkdf2_hmac_sha1, CMP_E_STR, true);
	REGISTER_TEST(_test2, CMP_E_STR, true);
    ASSERT_ALL();

    printf("%d out of %d tests complete!\n", successful_tests, TEST_NUMBER);
}