#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <sqlite3.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "base64.h"
#include "utils.h"
#include "encryption.h"
#include "defines.h"

#define PATH_CONFIG_DIR 0
#define PATH_FULL_DIR 1
#define PATH_HOME_DIR 2

extern char *__home_dir;
extern char *__config_dir;

struct db_info {
	sqlite3 *db_obj;
	char *table;
	char *username;
	char *password;
	char *config_dir;

	char salt[8];
	char *derived_key;
	char *master_password;
};

int __mkdir(char *dir);
bool sql3_cfg_dir_exists(char *dir);
bool sql3_db_exists_create(char *dir, char *db_name);
int sql3_db_init(sqlite3 **_db, char *db_name);
int sql3_db_close(sqlite3 *_db);
int sql3_table_create(struct db_info *database);
int sql3_table_list_tables(struct db_info *database);
int sql3_table_list_contents(struct db_info *database);
int sql3_table_delete(struct db_info *database);
int sql3_table_insert(struct db_info *database);

#endif
