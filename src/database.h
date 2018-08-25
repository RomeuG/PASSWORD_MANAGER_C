#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <sqlite3.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int __mkdir(char *dir);
bool sql3_cfg_dir_exists(char *dir);
char* sql3_get_db_full_path(char *dir, char* db_name);
bool sql3_db_exists_create(char *dir, char *db_name);
int sql3_db_init(sqlite3 **_db, char *db_name);
int sql3_db_close(sqlite3 *_db);
int sql3_table_create(sqlite3 *_db, char *table_name);
int sql3_table_list(sqlite3 *_db);

#endif