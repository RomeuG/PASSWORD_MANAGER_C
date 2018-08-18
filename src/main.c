#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defines.h"
#include "base64.h"
#include "encryption.h"

int main(int argc, char** argv) 
{
    // char* database_name;

    // if(argc != 2) {
    //     _Exit(EXIT_FAILURE);
    // }

    // database_name = argv[1];

    u8 key[AES_KEY_LENGTH] = {'t', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't'};
    u8 iv[AES_BLOCK_SIZE] = {'t', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 's'};

    u8 iv_enc[AES_BLOCK_SIZE];
    u8 iv_dec[AES_BLOCK_SIZE];

    memcpy(iv_enc, iv, AES_BLOCK_SIZE);
    memcpy(iv_dec, iv, AES_BLOCK_SIZE);

    u8 message[] = "This is a very big string to test out this AES encryption/decryption.";

    u8* encryption_output;
    u8* decryption_output;
    encryption_output = malloc(128);
    decryption_output = malloc(128);

    _AES_CBC_encrypt(message, encryption_output, sizeof(message), key, iv_enc);
    s8* b64_encoded = _b64_encode(encryption_output, sizeof(encryption_output));
    printf("%s\n", b64_encoded);

    _AES_CBC_decrypt(encryption_output, decryption_output, sizeof(message), key, iv_dec);

    printf("%s\n", decryption_output);

    free(b64_encoded);
    free(encryption_output);
    free(decryption_output);

    return EXIT_SUCCESS;
}