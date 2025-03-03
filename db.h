#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#include "args.h"

#define DB_NAME "test.db"

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"

void execute_sql(sqlite3 *db, const char *sql);
void create_if_not_exist(sqlite3 *db);

void add_task(sqlite3 *db, Arg *arg);
void delete_task(sqlite3 *db, Arg *arg);
void edit_task(sqlite3 *db, Arg *arg);
void mark_task(sqlite3 *db, Arg *arg);
void view_tasks(sqlite3 *db);

void print_tasks(sqlite3_stmt *stmt);

#endif
