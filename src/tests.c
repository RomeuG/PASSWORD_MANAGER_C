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

    _FREE(result);
    return true;
}

static bool __test_base64_encode_decode()
{
	u8 *b64_encoded;
	u8 *b64_decoded;
	char *text = "Hello World";

	_b64_encode(text, strlen(text), &b64_encoded);
	_b64_decode(b64_encoded, &b64_decoded);

	if (strcmp(text, b64_decoded) == 0) {
		return true;
	}

	return false;
}

static bool __test_full_encryption_decryption()
{
	s32 rc;

	char pass[] = "random_password";
	unsigned char* salt = "12345678";

	u8* message = (u8*)"HELLO WORLD!";
	u8 output_encryption[128] = {0};
	u8 output_decryption[128] = {0};

	// generate random IV
	u8 _rand_bytes[16];
	u8 _iv_enc[16];
	u8 _iv_dec[16];

	RAND_bytes(_rand_bytes, 16);
	memcpy(_iv_enc, _rand_bytes, 16);
	memcpy(_iv_dec, _rand_bytes, 16);

	// key derivation
	u8 *key_derived = malloc((2 * PBKDF2_OUTPUT_SIZE) + 1);
	if (!key_derived) goto __failed_ret__;

	bool res = PBKDF2_HMAC_SHA_X(pass, sizeof(pass), salt, PKCS5_SALT_LEN, key_derived);
	if (!res) goto __failed__;

	// encryption
	rc = _AES_CBC_encrypt((u8*)message, output_encryption, key_derived, _iv_enc);
	if (rc < 0) goto __failed__;

	// decryption
	rc = _AES_CBC_decrypt(output_encryption, output_decryption, key_derived, _iv_dec);
	if (rc < 0) goto __failed__;

	if (strcmp(output_decryption, message) == 0) {
		_FREE(key_derived);
		return true;
	}

__failed__:
	_FREE(key_derived);
__failed_ret__:
	return false;
}

void __tests()
{
	REGISTER_TEST(__test_pbkdf2_hmac_sha1, CMP_E_STR, true);
	REGISTER_TEST(__test_full_encryption_decryption, CMP_E_STR, true);
	REGISTER_TEST(__test_base64_encode_decode, CMP_E_STR, true);
	ASSERT_ALL();

	printf("%d out of %d tests complete!\n", successful_tests, TEST_NUMBER);
}
