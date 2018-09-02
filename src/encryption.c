#include "encryption.h"
#include "utils.h"

#define AES_128_CBC "aes-128-cbc"
#define AES_256_CBC "aes-256-cbc"
#define HASH_SHA1 "sha1"

#define PBKDF2_OUTPUT_SIZE 32
#define PBKDF2_ITERATIONS 1000

#define EVP_SHA1 EVP_sha1()
#define EVP_SHA256 EVP_sha256()
#define EVP_SHA512 EVP_sha512()

#define AES_SALT_LEN_8 8
#define AES_SALT_LEN_16 16

// TODO return something...
char* PBKDF2_HMAC_SHA_X(char* pass, unsigned char* salt, u8 salt_len, uint32_t output_bytes, char* hex_result)
{
	u32 i;
	s32 rc;
	char *HexResult = malloc(2 * PBKDF2_OUTPUT_SIZE);
	unsigned char digest[PBKDF2_OUTPUT_SIZE];

	rc = PKCS5_PBKDF2_HMAC(pass, strlen(pass), salt, salt_len, PBKDF2_ITERATIONS, EVP_SHA1, PBKDF2_OUTPUT_SIZE, digest);
	if (rc == 0) {
		DEBUG_PRINT("%s\n", "PKCS5_PBKDF2_HMAC() failed");
		return NULL;
	}

	for (i = 0; i < PBKDF2_OUTPUT_SIZE; i++) {
		sprintf(HexResult + (i * 2), "%02x", 255 & digest[i]);
	}

	return HexResult;
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
