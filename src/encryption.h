#ifndef __ENCRYPTION_H_
#define __ENCRYPTION_H_

#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#include "defines.h"

char* PBKDF2_HMAC_SHA_X(char* pass, unsigned char* salt, u8 salt_len, uint32_t output_bytes, char* hex_result);
s32 _AES_CBC_encrypt(u8 *message, u8 *buffer_out, size_t length, u8 *key, u8 *iv_enc);
s32 _AES_CBC_decrypt(u8 *message, u8 *buffer_out, size_t length, u8 *key, u8 *iv_dec);

#endif
