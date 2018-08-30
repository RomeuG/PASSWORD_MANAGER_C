#include "database.h"
#include "utils.h"
#include "defines.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define DIR_DATABASE "passshelter"
#define DIR_SEPARATOR "/"
#define DIR_HOME_CONFIG ".config"

#define DIR_PERMISSIONS 0700
#define DIR_MAX_SIZE 512

// TODO: should not drop table
#define SQL3_TABLE_CREATE_FORMAT_STRING "DROP TABLE IF EXISTS %s; CREATE TABLE %s (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, password TEXT NOT NULL);"
#define SQL3_TABLE_INSERT_FORMAT_STRING "INSERT INTO %s (username, password) VALUES(?1, ?2);"
#define SQL3_TABLE_DELETE_FORMAT_STRING "DELETE FROM %s WHERE ID = %s;"
#define SQL3_TABLE_DROP_FORMAT_STRING "DROP TABLE %s;"
#define SQL3_TABLE_LIST_FORMAT_STRING "SELECT name FROM sqlite_master WHERE type='table'"

int __mkdir(char *dir)
{
	return mkdir(dir, DIR_PERMISSIONS);
}

int sql3_cfg_create_file(char *full_path)
{
    return open(full_path, O_WRONLY | O_CREAT | O_NOCTTY | O_NONBLOCK, 0666); // TODO 0666
}

bool sql3_cfg_dir_exists(char *dir)
{
	DIR *_dir = opendir(dir);

	if (_dir) {
		return true;
	}

	// TODO return errno instead of a boolean
	return false;
}

// TODO this function will substitute sql3_cfg_get_path
static char *build_config_path(char *dir)
{
    //char *full_path = malloc(PATH_MAX);

    if (strcmp(dir, __home_dir) == 0) {
        // build path from $HOME
        strcat(dir, DIR_SEPARATOR);
        strcat(dir, DIR_HOME_CONFIG);

        goto _continue;

    } else if (strcmp(dir, __config_dir) == 0) {
        // build path from $HOME/.config
        _continue:
        strcat(dir, DIR_SEPARATOR);
        strcat(dir, DIR_DATABASE);

        if (!sql3_cfg_dir_exists(dir)) {
            if(__mkdir(dir) != 0) {
                DEBUG_PRINT("Errno - %d - %s\n", errno, strerror(errno));
                return NULL;
            }
        }

//        CHECK(sql3_cfg_dir_exists(dir), false, {
//                CHECK(__mkdir(dir), (0), return NULL, CMP_NE, "%s - %d\n", strerror(errno), errno);
//            }, CMP_E, "%s\n", "configuration directory does not exist");
    } else {
        DEBUG_PRINT("%s - %d\n", strerror(errno), errno);
        return NULL;
    }

    return dir;
}

// TODO function too big, creating two things in here is too much
bool sql3_db_exists_create(char *dir, char *db_name)
{
    int rc;
    char *full_path = build_config_path(dir);

	strcat(full_path, DIR_SEPARATOR);
	strcat(full_path, db_name);

	rc = access(full_path, F_OK);
	if (rc != -1) {
	    DEBUG_PRINT("%s\n", "database config already exists");
	    return true;
	}

	rc = sql3_cfg_create_file(full_path);
	if (rc >= 0) {
	    DEBUG_PRINT("%s\n", "database fil;e created successfully");
	    return true;
	}

//    CHECK(access(full_path, F_OK), (-1), return true,
//          CMP_NE, "%s\n", "database config already exists");
//    CHECK(sql3_cfg_create_file(full_path), (0), return true,
//            CMP_GE, "%s\n", "database file created successfully");

	return false;
}

// database open encapsulation
int sql3_db_init(sqlite3 **_db, char *db_name)
{
    return sqlite3_open_v2(db_name, _db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}

// database close encapsulation
int sql3_db_close(sqlite3 *_db)
{
    return sqlite3_close_v2(_db);
}

int sql3_table_create(sqlite3 *_db, char *table_name)
{
	int rc;
	char *query = NULL;

	rc = asprintf(&query, SQL3_TABLE_CREATE_FORMAT_STRING, table_name, table_name);
	if (rc <= 0) {
	    DEBUG_PRINT("%s - %d\n", "Error asprintf()", rc);
	    return rc;
	}

    rc = sqlite3_exec(_db, query, 0, 0, NULL);
	if(rc != SQLITE_OK) {
	    DEBUG_PRINT("%s - %d\n", sqlite3_errmsg(_db), rc);
	    free(query);
	    return rc;
	}

	free(query);
	return rc;
}

int sql3_table_list(sqlite3 *_db)
{
	int rc;
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(_db, SQL3_TABLE_LIST_FORMAT_STRING, -1, &stmt, NULL);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(stmt, 0));
    }

    if (rc == SQLITE_DONE) {
		DEBUG_PRINT("%s - %d\n", sqlite3_errmsg(_db), rc);
		return rc;
    }

    sqlite3_finalize(stmt);
    return rc;
}