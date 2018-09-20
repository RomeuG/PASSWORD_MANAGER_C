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

#define FILE_PERMISSIONS 0666
#define DIR_PERMISSIONS 0700
#define DIR_MAX_SIZE 512

// TODO: should not drop table
#define SQL3_TABLE_CREATE_FORMAT_STRING "DROP TABLE IF EXISTS %s; CREATE TABLE %s (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, password TEXT NOT NULL);"
#define SQL3_TABLE_INSERT_FORMAT_BINDING_STRING "INSERT INTO %s (username, password) VALUES(?1, ?2);"
#define SQL3_TABLE_INSERT_FORMAT_STRING "INSERT INTO %s (username, password) VALUES(\"%s\", \"%s\");"
#define SQL3_TABLE_DELETE_FORMAT_STRING "DELETE FROM %s WHERE ID = %s;"
#define SQL3_TABLE_DROP_FORMAT_STRING "DROP TABLE %s;"
#define SQL3_TABLE_LIST_FORMAT_STRING "SELECT name FROM sqlite_master WHERE type='table'"

#define NOT_NULL_OR_ABORT(var)					\
	do {										\
		if (var == NULL) {						\
			abort();							\
		}										\
	} while (0)


int __mkdir(char *dir)
{
	return mkdir(dir, DIR_PERMISSIONS);
}

int sql3_cfg_create_file(char *full_path)
{
	return open(full_path, O_WRONLY | O_CREAT | O_NOCTTY | O_NONBLOCK, FILE_PERMISSIONS); // TODO 0666
}

bool sql3_cfg_dir_exists(char *dir)
{
	DIR *_dir = opendir(dir);

	if (_dir) {
		_FREE(_dir);
		return true;
	}

	// TODO return errno instead of a boolean
	return false;
}

static char *build_config_path(char *dir)
{
    if (strcmp(dir, __home_dir) == 0) {
        // build path from $HOME
        strcat(dir, DIR_SEPARATOR);
        strcat(dir, DIR_HOME_CONFIG);

        goto __continue__;

    } else if (strcmp(dir, __config_dir) == 0) {
        // build path from $HOME/.config
	__continue__:
        strcat(dir, DIR_SEPARATOR);
        strcat(dir, DIR_DATABASE);

        if (!sql3_cfg_dir_exists(dir)) {
            if(__mkdir(dir) != 0) {
                DEBUG_PRINT(stderr, "Errno - %d - %s\n", errno, strerror(errno));
                return NULL;
            }
        }
    } else {
        DEBUG_PRINT(stderr, "%s - %d\n", strerror(errno), errno);
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
	    DEBUG_PRINT(stderr, "%s\n", "database config already exists");
	    return true;
	}

	rc = sql3_cfg_create_file(full_path);
	if (rc >= 0) {
	    DEBUG_PRINT(stderr, "%s\n", "database file created successfully");
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

int sql3_table_create(struct db_info *database)
{
	NOT_NULL_OR_ABORT(database);
	NOT_NULL_OR_ABORT(database->db_obj);
	NOT_NULL_OR_ABORT(database->table);

	int rc;
	char *query = NULL;

	rc = asprintf(&query, SQL3_TABLE_CREATE_FORMAT_STRING, database->table, database->table);
	if (rc <= 0) {
		DEBUG_PRINT(stderr, "%s - %d\n", "Error asprintf()", rc);
		return rc;
	}

	rc = sqlite3_exec(database->db_obj, query, 0, 0, NULL);
	if(rc != SQLITE_OK) {
		DEBUG_PRINT(stderr, "%s - %d\n", sqlite3_errmsg(database->db_obj), rc);
		_FREE(query);
		return rc;
	}

	_FREE(query);
	return rc;
}

int sql3_table_list_tables(struct db_info *database)
{
	NOT_NULL_OR_ABORT(database);
	NOT_NULL_OR_ABORT(database->db_obj);
	NOT_NULL_OR_ABORT(database->table);

	int rc;
	sqlite3_stmt *stmt;

	sqlite3_prepare_v2(database->db_obj, SQL3_TABLE_LIST_FORMAT_STRING, -1, &stmt, NULL);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		printf("%s\n", sqlite3_column_text(stmt, 0));
	}

	if (rc == SQLITE_DONE) {
		goto __finish__;
	}

__finish__:
	sqlite3_finalize(stmt);
	return rc;
}

int sql3_table_delete(struct db_info *database)
{
	NOT_NULL_OR_ABORT(database);
	NOT_NULL_OR_ABORT(database->db_obj);
	NOT_NULL_OR_ABORT(database->table);

	int rc;
	char *query = NULL;

	rc = asprintf(&query, SQL3_TABLE_DROP_FORMAT_STRING, database->table);
	if (rc <= 0) {
		DEBUG_PRINT(stderr, "%s - %d\n", "Error asprintf()", rc);
		return rc;
	}

	rc = sqlite3_exec(database->db_obj, query, 0, 0, NULL);
	if (rc != SQLITE_OK) {
		DEBUG_PRINT(stderr, "%s - %d\n", sqlite3_errmsg(database->db_obj), rc);
		_FREE(query);
		return rc;
	}

	_FREE(query);
	return rc;
}

// TODO: change it to use bindings
int sql3_table_insert(struct db_info *database)
{
	NOT_NULL_OR_ABORT(database);
	NOT_NULL_OR_ABORT(database->db_obj);
	NOT_NULL_OR_ABORT(database->username);
	NOT_NULL_OR_ABORT(database->password);

	int rc;
	char *query = NULL;

	// TODO: encrypt password

	rc = asprintf(&query, SQL3_TABLE_INSERT_FORMAT_STRING, database->table, database->username, database->password);
	if (rc <= 0) {
		DEBUG_PRINT(stderr, "%s - %d\n", "Error asprintf()", rc);
        return rc;
    }

    rc = sqlite3_exec(database->db_obj, query, 0, 0, NULL);
    if (rc != SQLITE_OK) {
		DEBUG_PRINT(stderr, "%s - %d\n", sqlite3_errmsg(database->db_obj), rc);
		_FREE(query);
        return rc;
    }

    _FREE(query);
    return rc;
}
