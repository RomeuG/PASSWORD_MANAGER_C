#include "database.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#define DIR_DATABASE "passshelter"
#define DIR_SEPARATOR "/"
#define DIR_PERMISSIONS 0700

int __mkdir(char *dir)
{
	return mkdir(dir, DIR_PERMISSIONS);
}

bool sql3_cfg_dir_exists(char *dir)
{
	DIR *_dir = opendir(dir);

	if (_dir) {
		return true;
	}

	return errno;
}

bool sql3_db_exists(char *dir, char *db_name)
{
  	char full_path[256];

	// use safe versions of these functions
	strcpy(full_path, dir);
	strcat(full_path, DIR_SEPARATOR);
	strcat(full_path, DIR_DATABASE);

	// check if $HOME/.config/passshelter exists
	if (!sql3_cfg_dir_exists(dir)) {
		__mkdir(dir);
	}

	// concatenate db file name
	strcat(full_path, DIR_SEPARATOR);
	strcat(full_path, db_name);

  	if (access(full_path, F_OK) != -1) {
    	return true;
	}

	return false;
}

// database open encapsulation
int sql3_db_init(sqlite3 *_db, char *db_name) 
{
    return sqlite3_open_v2(db_name, &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}

// database close encapsulation
int sql3_db_close(sqlite3 *_db) 
{
    return sqlite3_close_v2(_db);
}

int sql3_list_tables(sqlite3 *_db) 
{
	sqlite3_stmt *stmt;
	return sqlite3_prepare_v2(_db, SQL_LIST_TABLES, -1, &stmt, NULL);
}