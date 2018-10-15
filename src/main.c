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

// TODO #7
#pragma GCC poison

#define EV_CONFIG_DIR "XDG_CONFIG_HOME"
#define EV_HOME_DIR "HOME"
#define DATABASE_NAME "pass_shelter.db"

#define PATH_SIZE (1 << 10)

#define NO_ARG 0
#define REQ_ARG 1
#define OPT_ARG 2

#define ARGS_NONE 0
#define ARGS_ADD 1
#define ARGS_CREATE 2
#define ARGS_DELETE 3
#define ARGS_LIST 4
#define ARGS_HELP 5
#define ARGS_TESTS CHAR_MAX

char *__home_dir;
char *__config_dir;

// getopt_long flags
struct getopt_flags {
	s32 add;
	s32 create;
	s32 delete;
	s32 list;
	s32 username;
	s32 password;
	s32 help;
	s32 tests;
};

static void print_flags(struct getopt_flags *flags)
{
	printf("%s:%d\n%s:%d\n"
		   "%s:%d\n%s:%d\n"
		   "%s:%d\n%s:%d\n"
		   "%s:%d\n%s:%d\n",
		   "Add", flags->add,
		   "Create", flags->create,
		   "Delete", flags->delete,
		   "List", flags->list,
		   "Username", flags->username,
		   "Password", flags->password,
		   "Help", flags->help,
		   "Tests", flags->tests);
}

static bool load_env_variables()
{
    __home_dir = __getenv(EV_HOME_DIR);
    if (__home_dir == NULL) goto __error__;

    __config_dir = __getenv(EV_CONFIG_DIR);
    if (__config_dir == NULL) goto __error__;

    return true;

__error__:
    return false;
}

u8 flags_check(struct getopt_flags *flags)
{
	if (flags->add == 1 &&
		flags->username == 1 &&
		flags->password == 1) {
		return ARGS_ADD;
	} else if (flags->create == 1) {
		return ARGS_CREATE;
	} else if (flags->delete == 1) {
		return ARGS_DELETE;
	} else if (flags->list == 1) {
		return ARGS_LIST;
	} else if (flags->help == 1) {
		return ARGS_HELP;
	} else if (flags->tests == 1) {
		return ARGS_TESTS;
	}

	return ARGS_NONE;
}

// TODO #8
void print_help()
{
	printf("Usage:\n./program -t\n");
}

// encryption operations
bool encryption_init(struct db_info *db)
{
	bool res;

	db->derived_key = malloc((PBKDF2_OUTPUT_SIZE * 2) + 1);

	res = PBKDF2_HMAC_SHA_X(db->password, sizeof(db->password), db->salt, PKCS5_SALT_LEN, db->derived_key);
	if (!res) return false;

	return true;
}

int main(int argc, char** argv, char **envp)
{
	u8 res;
	int rc;
	int copts;
	//char *default_available_path = NULL;

	// db struct
	struct db_info database = {0};
	// arg flags
	static struct getopt_flags arg_flags = {0};

	// setup long options
	int long_index = 0;
	static struct option long_options[] = {
		{"add",      NO_ARG,  &arg_flags.add,      'a'},
		{"create",   REQ_ARG, &arg_flags.create,   'c'},
		{"delete",   REQ_ARG, &arg_flags.delete,   'd'},
		{"list",     NO_ARG,  &arg_flags.list,     'l'},
		{"password", REQ_ARG, &arg_flags.password, 'p'},
		{"username", REQ_ARG, &arg_flags.username, 'u'},
		{"tests",    NO_ARG,  &arg_flags.tests,    't'},
		{0,          0,       0,                    0 },
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

    database.config_dir = malloc(PATH_SIZE);
    strcpy(database.config_dir, __config_dir ? __config_dir : __home_dir);

    rc = sql3_db_exists_create(database.config_dir, DATABASE_NAME);
    if (!rc) {
        DEBUG_PRINT(stderr, "%s\n", "problems creating database file...\nExiting...");
        _FREE(database.config_dir);
        exit(EXIT_FAILURE);
    }

	rc = sql3_db_init(&database.db_obj, database.config_dir);
	if (rc != SQLITE_OK) {
		DEBUG_PRINT(stderr, "%s\n", sqlite3_errmsg(database.db_obj));
		_FREE(database.config_dir);
		exit(EXIT_FAILURE);
	}

	encryption_init(&database);
	// TODO: remove
	memcpy(database.salt, "11111111", 8);

	// command line options
	while ((copts = getopt_long(argc, argv, "a:c:d:hlptu:", long_options, &long_index)) != -1) {
		switch (copts) {
		case 'a':
			arg_flags.add = 1;
			database.table = strdup(optarg);
			break;
		case 'c':
			arg_flags.create = 1;
			database.table = strdup(optarg);
			break;
		case 'd':
			arg_flags.delete = 1;
			break;
		case 'h':
			arg_flags.help = 1;
			break;
		case 'l':
			arg_flags.list = 1;
			break;
		case 'p':
			arg_flags.password = 1;
			database.password = getpass("Insert password: ");
			break;
		case 'u':
			arg_flags.username = 1;
			database.username = strdup(optarg);
			break;
		case 't':
			arg_flags.tests = 1;
			break;
		default:
			break;
		}
	}

	// check flags
	res = flags_check(&arg_flags);
	// TODO #2
	switch (res) {
	case ARGS_NONE:
		DEBUG_PRINT(stdout, "%s\n", "No arguments used.");
		print_flags(&arg_flags);
		break;
	case ARGS_ADD:
		rc = sql3_table_insert(&database);
		break;
	case ARGS_CREATE:
		rc = sql3_table_create(&database);
		break;
	case ARGS_DELETE:
		rc = sql3_table_delete(&database);
		break;
	case ARGS_LIST:
		rc = sql3_table_list_tables(&database);
		break;
	case ARGS_HELP:
		print_help();
		break;
	case ARGS_TESTS:
		__tests();
		break;
	default:
		DEBUG_PRINT(stderr, "%s\n", "Invalid argument type.");
		break;
	}

	if (rc != SQLITE_OK) {
		DEBUG_PRINT(stderr, "%s\n", "An error ocurred while completing your SQL action.");
	}

	sql3_db_close(database.db_obj);

	_FREE(database.table);
	_FREE(database.username);
	_FREE(database.password);
	_FREE(database.config_dir);
	_FREE(database.derived_key);

	return EXIT_SUCCESS;
}
