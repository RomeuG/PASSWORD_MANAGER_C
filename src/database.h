#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <sqlite3.h>

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define SQL_LIST_TABLES ".tables"

bool sql3_db_exists(char *db_name);
int sql3_db_init(sqlite3 *_db, char *db_name);
int sql3_db_close(sqlite3 *_db);
int sql3_list_tables(sqlite3 *_db);

#endif