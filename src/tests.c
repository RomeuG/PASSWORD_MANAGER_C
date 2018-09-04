#include "tests.h"

void __tests()
{
    char pass[] = "random_password";
    unsigned char* salt = "12345678";

    char *result = malloc(2 * PBKDF2_OUTPUT_SIZE);

    bool res = PBKDF2_HMAC_SHA_X(pass, sizeof(pass), salt, PKCS5_SALT_LEN, result);
    if (!res) {
        DEBUG_PRINT("%s\n", "test failed");
        return;
    }

    printf("Hex result: %s\n", result);
}