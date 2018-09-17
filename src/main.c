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

#include "tests.h"

// TODO use pragma poison
#pragma GCC poison

#define EV_CONFIG_DIR "XDG_CONFIG_HOME"
#define EV_HOME_DIR "HOME"
#define DATABASE_NAME "pass_shelter.db"

char *__home_dir;
char *__config_dir;

// TODO more elegant error handling
static bool load_env_variables()
{
    __home_dir = __getenv(EV_HOME_DIR);
    if (__home_dir == NULL) goto error;

    __config_dir = __getenv(EV_CONFIG_DIR);
    if (__config_dir == NULL) goto error;

    return true;

    error:
    return false;
}

int main(int argc, char** argv, char **envp)
{
	int rc;
	int copts;
	sqlite3 *db;
	char *default_available_path = NULL;

	// init some ssl stuff
	OpenSSL_add_all_algorithms();

	// load environment variables
	if (!load_env_variables()) {
		if (__home_dir == NULL && __config_dir == NULL) {
			DEBUG_PRINT(stderr, "%s\n", "no environment variables found");
			exit(EXIT_FAILURE);
		}
	}

    // TODO: allocate less bytes. Current = 4kb
    default_available_path = malloc(PATH_MAX);
    strcpy(default_available_path, __config_dir ? __config_dir : __home_dir);

    rc = sql3_db_exists_create(default_available_path, DATABASE_NAME);
    if (!rc) {
        DEBUG_PRINT(stderr, "%s\n", "problems creating database file...\nExiting...");
        free(default_available_path);
        exit(EXIT_FAILURE);
    }

	rc = sql3_db_init(&db, default_available_path);
    if (rc != SQLITE_OK) {
        DEBUG_PRINT(stderr, "%s\n", sqlite3_errmsg(db));
        free(default_available_path);
        exit(EXIT_FAILURE);
	}

	// TODO convert to LONG_OPTIONS
	// command line options
	while ((copts = getopt(argc, argv, "a:d:l:t")) != -1) {
		switch (copts) {
		case 'a':
			//add account
			sql3_table_insert(db, "table", "username", "password");
			break;
		case 'c':
			sql3_table_create(db, optarg);
			break;
		case 'd':
			sql3_table_delete(db, optarg);
			break;
		case 'l':
			sql3_table_list(db);
			break;
        case 't':
			__tests();
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	sql3_db_close(db);

	free(default_available_path);

    return EXIT_SUCCESS;
}
