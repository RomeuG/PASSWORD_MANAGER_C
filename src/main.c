#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) 
{
    u8 key[AES_KEY_LENGTH] = {'t', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't'};
    u8 iv[AES_BLOCK_SIZE] = {'t', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 't', 't', 'e', 's', 's'};


    if(argc != 2) {
        _Exit(EXIT_FAILURE);
    }

    database_name = argv[1];
}