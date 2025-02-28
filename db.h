#ifndef DB_H
#define DB_H

#include <sqlite3.h>

#define DB_NAME "test.db"

void execute_sql(sqlite3 *db, const char *sql);
void create_if_not_exist(sqlite3 *db);

void add_task(sqlite3 *db, const char *title, const char *description);
void delete_task(sqlite3 *db, int id);
void edit_task(sqlite3 *db, int id, const char *title, const char *description);
void view_tasks(sqlite3 *db);


#endif
