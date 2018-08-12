#include <sqlite3.h>
#include <stdio.h>

sqlite3 *db;

// database open encapsulation
int sql3_db_init(sqlite3 *_db, char *db_name) 
{
    int error_check;
    
    error_check = sqlite3_open_v2(db_name, &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if(error_check != SQLITE_OK) {
        return error_check;
    }

    return SQLITE_OK;
}

// database close
int sql3_db_close(sqlite3 *_db) 
{
    int error_check;

    error_check = sqlite3_close_v2(_db);

    if(error_check != SQLITE_OK) {
        return error_check;
    }

    return SQLITE_OK;
}