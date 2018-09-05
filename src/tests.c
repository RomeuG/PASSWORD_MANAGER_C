#include <assert.h>
#include "tests.h"

#define assert__(function, operation, expected) { \
    assert(function() operation expected);\
}

u8 i;

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
    assert__(__test_pbkdf2_hmac_sha1, CMP_E, true);
}