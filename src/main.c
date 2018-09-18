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

#define NO_ARG 0
#define REQ_ARG 1
#define OPT_ARG 2

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

	// getopt_long flags
	typedef struct {
	    s32 add;
	    s32 create;
	    s32 delete;
	    s32 list;
        s32 username;
        s32 password;
        s32 tests;
	} _getopt_flags;

	static _getopt_flags _flags = {0};
	// setup long options
	int long_index = 0;
	static struct option long_options[] = {
			{"add",      NO_ARG,  &_flags.add, 'a'},
			{"create",   REQ_ARG, &_flags.create, 'c'},
			{"delete",   REQ_ARG, &_flags.delete, 'd'},
			{"list",     NO_ARG,  &_flags.list, 'l'},
			{"password", REQ_ARG, &_flags.password, 'p'},
			{"username", REQ_ARG, &_flags.username, 'u'},
			{"tests",    NO_ARG,  &_flags.tests, 't'},
			{0,          0,       0, 0  },
	};

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

	// command line options
	while ((copts = getopt_long(argc, argv, "a:d:p:l:t", long_options, &long_index)) != -1) {
        switch (copts) {
            case 'a': break;
            case 'c': break;
            case 'd': break;
            case 'l': break;
            case 'p': break;
            case 'u': break;
            case 't': break;
            default:
                exit(EXIT_FAILURE);
        }
	}

	sql3_db_close(db);

	free(default_available_path);

    return EXIT_SUCCESS;
}
