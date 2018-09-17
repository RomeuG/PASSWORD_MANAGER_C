#ifndef __ENCRYPTION_H_
#define __ENCRYPTION_H_

#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdbool.h>

#include "defines.h"

bool PBKDF2_HMAC_SHA_X(char* pass, u8 pass_len, unsigned char* salt, u8 salt_len, u8* hex_result);
s32 _AES_CBC_encrypt(u8 *message, u8 *buffer_out, u8 *key, u8 *iv_enc);
s32 _AES_CBC_decrypt(u8 *message, u8 *buffer_out, u8 *key, u8 *iv_dec);

#endif
