#ifndef __ENCRYPTION_H_
#define __ENCRYPTION_H_

#include <stdio.h>
#include <openssl/aes.h>

#include "defines.h"

u8 _AES_CBC_encrypt(u8 *message, u8 *buffer_out, u8 *key, u8 *iv_enc);
u8 _AES_CBC_decrypt(u8 *message, u8 *buffer_out, u8 *key, u8 *iv_dec);

#endif