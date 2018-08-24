#include "encryption.h"

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