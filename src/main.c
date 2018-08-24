#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <getopt.h>

#include "defines.h"
#include "base64.h"
#include "encryption.h"
#include "database.h"
#include "utils.h"

#define CONFIG_DIR "XDG_CONFIG_HOME"
#define DATABASE_NAME "pass_shelter.db"

int main(int argc, char** argv) 
{
    int copts;
    char* config_dir = __getenv(CONFIG_DIR);

    if (!sql3_db_exists_create(config_dir, DATABASE_NAME)) {
        DEBUG_PRINT("%s", "Problems creating database file...\nExiting...\n");
        exit(EXIT_FAILURE);
    }

    while ((copts = getopt(argc, argv, "l:")) != -1) {
        switch (copts) {
            case 'l':
                //sql3_list_tables();
                break;
            default:
                exit(EXIT_FAILURE);
      }
    }

    return EXIT_SUCCESS;
}