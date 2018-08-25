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
#define DIR_PERMISSIONS 0700
#define DIR_MAX_SIZE 512

#define PATH_CONFIG_DIR 0
#define PATH_FULL_PATH 1

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

int sql3_create_db_file(char *full_path)
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

char *sql3_cfg_get_path(char *dir, char *db_name, u8 type)
{
    char *full_path = malloc(DIR_MAX_SIZE);

    if (type == PATH_CONFIG_DIR) {
		// use safe versions of these functions
		strcpy(full_path, dir);
		strcat(full_path, DIR_SEPARATOR);
		strcat(full_path, DIR_DATABASE);
    } else {
		// use safe versions of these functions
		strcpy(full_path, dir);
		strcat(full_path, DIR_SEPARATOR);
		strcat(full_path, DIR_DATABASE);

		// concatenate db file name
		strcat(full_path, DIR_SEPARATOR);
		strcat(full_path, db_name);
    }

    return full_path;
}

// TODO function too big, creating two things in here is too much
bool sql3_db_exists_create(char *dir, char *db_name)
{
  	char full_path[256];

	// use safe versions of these functions
	strcpy(full_path, dir);
	strcat(full_path, DIR_SEPARATOR);
	strcat(full_path, DIR_DATABASE);

	// check if $HOME/.config/passshelter exists
	if (!sql3_cfg_dir_exists(full_path)) {
		if(__mkdir(full_path) != 0) {
		    DEBUG_PRINT("Errno - %d - %s\n", errno, strerror(errno));
            return false;
		}
	}

	// concatenate db file name
	strcat(full_path, DIR_SEPARATOR);
	strcat(full_path, db_name);

  	if (access(full_path, F_OK) != -1) {
    	return true;
	}

	if (sql3_create_db_file(full_path) >= 0) {
        return true;
    }

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
		return !SQLITE_DONE;
    }

    sqlite3_finalize(stmt);
    return rc;
}