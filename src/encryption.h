#ifndef __ENCRYPTION_H_
#define __ENCRYPTION_H_

#include <stdio.h>
#include <openssl/aes.h>

#include "defines.h"

s32 _AES_CBC_encrypt(u8 *message, u8 *buffer_out, size_t length, u8 *key, u8 *iv_enc);
s32 _AES_CBC_decrypt(u8 *message, u8 *buffer_out, size_t length, u8 *key, u8 *iv_dec);

#endif