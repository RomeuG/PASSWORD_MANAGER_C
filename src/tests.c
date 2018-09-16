#include "tests.h"

u8 successful_tests = 0;
u8 tests_registered = 0;

static bool __test_pbkdf2_hmac_sha1()
{
    char pass[] = "random_password";
    unsigned char* salt = "12345678";

    // +1 for null terminator
    char *result = malloc((2 * PBKDF2_OUTPUT_SIZE) + 1);

    bool res = PBKDF2_HMAC_SHA_X(pass, sizeof(pass), salt, PKCS5_SALT_LEN, result);
    if (!res) {
        return false;
    }

    free(result);
    return true;
}

static bool __test_full_encryption_decryption()
{
	s32 rc;

	char pass[] = "random_password";
	unsigned char* salt = "12345678";

	char* message = "HELLO WORLD!";
	unsigned char output_encryption[128] = {0};
	unsigned char output_decryption[128] = {0};

	// generate random IV
	u8 _iv_enc[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	u8 _iv_dec[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	//RAND_bytes(_iv, 16);

	// key derivation
	char *result = malloc((2 * PBKDF2_OUTPUT_SIZE) + 1);

	bool res = PBKDF2_HMAC_SHA_X(pass, sizeof(pass), salt, PKCS5_SALT_LEN, result);
	if (!res) return false;

	// encryption
	rc = _AES_CBC_encrypt((u8*)message, output_encryption, result, _iv_enc);
	if (rc < 0) return false;

	// decryption
	rc = _AES_CBC_decrypt(output_encryption, output_decryption, strlen(output_encryption), result, _iv_dec);
	if (rc < 0) return false;

	return true;
}

void __tests()
{
	REGISTER_TEST(__test_pbkdf2_hmac_sha1, CMP_E_STR, true);
	REGISTER_TEST(__test_full_encryption_decryption, CMP_E_STR, true);
	ASSERT_ALL();

	printf("%d out of %d tests complete!\n", successful_tests, TEST_NUMBER);
}
