#include "encryption.h"
#include "utils.h"

bool PBKDF2_HMAC_SHA_X(char* pass, u8 pass_len, unsigned char* salt, u8 salt_len, char* hex_result)
{
	u32 i;
	s32 rc;
	unsigned char digest[PBKDF2_OUTPUT_SIZE];

	if (!hex_result) {
		DEBUG_PRINT(stderr, "%s\n", "variable for result is NULL");
		return false;
	}

	rc = PKCS5_PBKDF2_HMAC(pass, pass_len, salt, salt_len, PBKDF2_ITERATIONS, EVP_SHA1, PBKDF2_OUTPUT_SIZE, digest);
	if (rc == 0) {
		DEBUG_PRINT(stderr, "%s\n", "PKCS5_PBKDF2_HMAC() failed");
		return false;
	}

	for (i = 0; i < PBKDF2_OUTPUT_SIZE; i++) {
		sprintf(hex_result + (i * 2), "%02x", 255 & digest[i]);
	}

	return true;
}

s32 _AES_CBC_encrypt(u8 *message, u8 *buffer_out, size_t length, u8 *key, u8 *iv_enc)
{
	s32 res;

	AES_KEY encryption_key;
	res = AES_set_encrypt_key(key, AES_KEY_LENGTH * 8, &encryption_key);

	if (res < 0) {
		printf("ERROR");
		return res;
	}

	AES_cbc_encrypt(message, buffer_out, length, &encryption_key, iv_enc, AES_ENCRYPT);

	return res;
}

s32 _AES_CBC_decrypt(u8 *message, u8 *buffer_out, size_t length, u8 *key, u8 *iv_dec)
{
	s32 res;

	AES_KEY decryption_key;
	res = AES_set_decrypt_key(key, AES_KEY_LENGTH * 8, &decryption_key);

	if (res < 0) {
		printf("ERROR");
		return res;
	}

	AES_cbc_encrypt(message, buffer_out, length, &decryption_key, iv_dec, AES_DECRYPT);

	return res;
}
