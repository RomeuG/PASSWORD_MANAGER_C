#include "encryption.h"
#include "utils.h"

#define TO_HEX(src, dest, n)							\
	for (u32 i = 0; i < n; i++) {						\
		sprintf(dest + (i * 2), "%02x", 255 & src[i]);	\
	}

void SSL_print_error()
{
	char err[128];
	ERR_load_crypto_strings();
	ERR_error_string(ERR_get_error(), err);
	DEBUG_PRINT(stderr, "%s\n", err);
}

bool PBKDF2_HMAC_SHA_X(char* pass, u8 pass_len, unsigned char* salt, u8 salt_len, u8* hex_result)
{
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

	//convert_to_hex(digest, hex_result, PBKDF2_OUTPUT_SIZE);
	TO_HEX(digest, hex_result, PBKDF2_OUTPUT_SIZE);

	return true;
}

s32 _AES_CBC_encrypt(u8 *message, u8 *buffer_out, u8 *key, u8 *iv_enc)
{
	s32 res;

	AES_KEY encryption_key;
	res = AES_set_encrypt_key(key, AES_KEY_LENGTH * 4, &encryption_key);

	if (res < 0) {
		DEBUG_PRINT(stderr, "%s\n", "AES_set_encrypt_key() error");
		return res;
	}

	AES_cbc_encrypt(message, buffer_out, AES_KEY_LENGTH, &encryption_key, iv_enc, AES_ENCRYPT);
	return res;
}

s32 _AES_CBC_decrypt(u8 *message, u8 *buffer_out, u8 *key, u8 *iv_dec)
{
	s32 res;

	AES_KEY decryption_key;
	res = AES_set_decrypt_key(key, AES_KEY_LENGTH * 4, &decryption_key);

	if (res < 0) {
		DEBUG_PRINT(stderr, "%s\n", "AES_set_decrypt_key()");
		return res;
	}

	AES_cbc_encrypt(message, buffer_out, AES_KEY_LENGTH, &decryption_key, iv_dec, AES_DECRYPT);
	return res;
}
