#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <getopt.h>
#include <limits.h>

#include "defines.h"
#include "base64.h"
#include "encryption.h"
#include "database.h"
#include "utils.h"

#define EV_CONFIG_DIR "XDG_CONFIG_HOME"
#define EV_HOME_DIR "HOME"
#define DATABASE_NAME "pass_shelter.db"

char *__home_dir;
char *__config_dir;

// TODO more elegant error handling
u8 load_env_variables()
{
    __home_dir = __getenv(EV_HOME_DIR);
    if (__home_dir == NULL) goto error;

    __config_dir = __getenv(EV_CONFIG_DIR);
    if (__config_dir == NULL) goto error;

    return 0;

    error:
    return 1;
}

int main(int argc, char** argv, char **envp)
{
    int rc;
    int copts;
    sqlite3 *db;
    char *default_available_path = NULL;

    if (!load_env_variables()) {
        if (__home_dir == NULL && __config_dir == NULL) {
            DEBUG_PRINT("%s\n", "No environment variables found.");
            exit(EXIT_FAILURE);
        }
    }

    // allocate less bytes. Current = 4kb
    default_available_path = malloc(PATH_MAX);
    strcpy(default_available_path, __config_dir ? __config_dir : __home_dir);

    // TODO macro to do the checking
    if (!sql3_db_exists_create(default_available_path, DATABASE_NAME)) {
        DEBUG_PRINT("%s", "Problems creating database file...\nExiting...\n");
        exit(EXIT_FAILURE);
    }

    // TODO macro to do the checking
    rc = sql3_db_init(&db, default_available_path);
    if (rc != SQLITE_OK) {
        DEBUG_PRINT("%s - %d\n", sqlite3_errmsg(db), rc);
    }

    while ((copts = getopt(argc, argv, "l:")) != -1) {
        switch (copts) {
            case 'l':
                sql3_table_create(db, "TEST_DB");
                sql3_table_list(db);
                break;
            default:
                exit(EXIT_FAILURE);
      }
    }

    sql3_db_close(db);

    return EXIT_SUCCESS;
}