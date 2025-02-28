#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

void execute_sql(sqlite3 *db, const char *sql) {
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void create_if_not_exist(sqlite3 *db) {
    char *sql = "CREATE TABLE IF NOT EXISTS tasks ( id INTEGER PRIMARY KEY "
                "AUTOINCREMENT, title TEXT "
                "NOT NULL, description TEXT, status TEXT DEFAULT 'pending');";

    execute_sql(db, (const char *)sql);
}

void add_task(sqlite3 *db, const char *title, const char *description) {
    char sql[256];
    if (description && strlen(description) > 0 && strlen(description) < 255) {
        snprintf(sql, sizeof(sql),
                 "INSERT INTO tasks (title, description) VALUES ('%s', '%s')",
                 title, description);
        printf("Added task '%s' with description '%s'\n", title, description);
    } else {
        snprintf(sql, sizeof(sql), "INSERT INTO tasks (title) VALUES ('%s')",
                 title);
        printf("Added task '%s'\n", title);
    }

    execute_sql(db, sql);
}

void delete_task(sqlite3 *db, int id) {
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM tasks WHERE id = %d", id);

    execute_sql(db, sql);
    printf("Deleted task with ID: %d\n", id);
}

void edit_task(sqlite3 *db, int id, const char *title,
               const char *description) {
    char sql[256];
    if (description && strlen(description) > 0 && strlen(description) < 255) {
        snprintf(
            sql, sizeof(sql),
            "UPDATE tasks SET title = '%s', description = '%s' WHERE id = %d",
            title, description, id);

    } else {
        snprintf(sql, sizeof(sql),
                 "UPDATE tasks SET title = '%s' WHERE id = %d", title, id);
    }

    execute_sql(db, sql);
}

void view_tasks(sqlite3 *db) {
    const char *sql = "SELECT * FROM tasks";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("No tasks found.\n");
        sqlite3_finalize(stmt);
        return;
    }

    printf("Tasks:\n");
    do {
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *)sqlite3_column_text(stmt, 1);
        const char *description = (const char *)sqlite3_column_text(stmt, 2);
        const char *status = (const char *)sqlite3_column_text(stmt, 3);

        if (description) {
            printf("ID: %d, Title: %s, Description: %s, Status: %s\n", id,
                   title, description, status);
        } else {
            printf("ID: %d, Title: %s, Status: %s\n", id, title, status);
        }

    } while (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
}
