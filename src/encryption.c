#include <stdio.h>

#include <openssl/aes.h>

#include "defines.h"

uint8 _AES_CBC_encrypt(uint8 *message, uint8 *buffer_out, uint8 *key, uint8 *iv_enc)
{
    int8 res;

    AES_KEY encryption_key;
    res = AES_set_encrypt_key(key, AES_KEY_LENGTH * 8, &encryption_key);

    if (res < 0) {
        printf("ERROR");
        return res;
    }

    AES_cbc_encrypt(message, buffer_out, sizeof(message), &encryption_key, iv_enc, AES_ENCRYPT);

    return res;
}

uint8 _AES_CBC_decrypt(uint8 *message, uint8 *buffer_out, uint8 *key, uint8 *iv_dec)
{
    int8 res;

    AES_KEY decryption_key;
    res = AES_set_decrypt_key(key, AES_KEY_LENGTH * 8, &decryption_key);

    if (res < 0) {
        printf("ERROR");
        return res;
    }

    AES_cbc_encrypt(message, buffer_out, sizeof(message), &decryption_key, iv_dec, AES_DECRYPT);

    return res;
}