#include <sqlite3.h>

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

bool sql3_db_exists(char *db_name)
{
	// struct stat   buffer;   
  	// return (stat (filename, &buffer) == 0);

	if (access(db_name, F_OK) != -1) {
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