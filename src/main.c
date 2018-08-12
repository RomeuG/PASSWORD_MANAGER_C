#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) 
{
    char* database_name;

    if(argc != 2) {
        _Exit(EXIT_FAILURE);
    }

    database_name = argv[1];
}