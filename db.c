#include "db.h"
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
    const char *sql =
        "CREATE TABLE IF NOT EXISTS tasks ( id INTEGER PRIMARY KEY "
        "AUTOINCREMENT, title TEXT "
        "NOT NULL, description TEXT, status TEXT DEFAULT 'pending');";

    execute_sql(db, sql);
}

void add_task(sqlite3 *db, Arg *arg) {
    char sql[256];
    if (arg->description && strlen(arg->description) > 0 &&
        strlen(arg->description) < 255) {
        snprintf(sql, sizeof(sql),
                 "INSERT INTO tasks (title, description) VALUES ('%s', '%s')",
                 arg->title, arg->description);
        if (arg->verbose)
            printf("Added task '%s' with description '%s'\n", arg->title,
                   arg->description);
    } else {
        snprintf(sql, sizeof(sql), "INSERT INTO tasks (title) VALUES ('%s')",
                 arg->title);
        if (arg->verbose)
            printf("Added task '%s' in default status 'pending'\n", arg->title);
    }

    execute_sql(db, sql);
}

void delete_task(sqlite3 *db, Arg *arg) {
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM tasks WHERE id = %d", arg->id);

    execute_sql(db, sql);
    if (arg->verbose)
        printf("Deleted task with ID: %d\n", arg->id);
}

void edit_task(sqlite3 *db, Arg *arg) {
    char sql[256];
    if (arg->description && strlen(arg->description) > 0 &&
        strlen(arg->description) < 255) {
        snprintf(
            sql, sizeof(sql),
            "UPDATE tasks SET title = '%s', description = '%s' WHERE id = %d",
            arg->title, arg->description, arg->id);

        if (arg->verbose) {
            printf("Edited task with ID: %d\n", arg->id);
            printf("Title: %s\n", arg->title);
            printf("Description: %s\n", arg->description);
        }

    } else {
        snprintf(sql, sizeof(sql),
                 "UPDATE tasks SET title = '%s' WHERE id = %d", arg->title,
                 arg->id);

        if (arg->verbose) {
            printf("Edited task with ID: %d\n", arg->id);
            printf("Title: %s\n", arg->title);
        }
    }

    execute_sql(db, sql);
}

void mark_task(sqlite3 *db, Arg *arg) {
    if (strcmp(arg->status, "pending") == 0 ||
        strcmp(arg->status, "done") == 0 ||
        strcmp(arg->status, "in_progress") == 0 ||
        strcmp(arg->status, "failed") == 0 ||
        strcmp(arg->status, "cancelled") == 0) {

        char sql[256];
        snprintf(sql, sizeof(sql),
                 "UPDATE tasks SET status = '%s' WHERE id = %d", arg->status,
                 arg->id);

        execute_sql(db, sql);

        if (arg->verbose)
            printf("Marked task with ID: %d as %s\n", arg->id, arg->status);
    }
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
        printf("Use 'add' command to add a task.\n");
        sqlite3_finalize(stmt);
        return;
    }

    print_tasks(stmt);

    sqlite3_finalize(stmt);
}

void print_tasks(sqlite3_stmt *stmt) {
    printf(CYAN BOLD "┌────────┬─────────────────────┬─────────────────────"
                     "────────┬──────────┐\n" RESET);
    printf(CYAN BOLD "│   ID   │        Title        │        Description  "
                     "        │  Status  │\n" RESET);
    printf(CYAN BOLD "├────────┼─────────────────────┼─────────────────────"
                     "────────┼──────────┤\n" RESET);

    do {
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *)sqlite3_column_text(stmt, 1);
        const char *description = (const char *)sqlite3_column_text(stmt, 2);
        const char *status = (const char *)sqlite3_column_text(stmt, 3);

        if (!title)
            title = "";
        if (!description)
            description = "";
        if (!status)
            status = "unknown";

        // Цвет статуса
        const char *status_color = RESET;
        if (strcmp(status, "done") == 0) {
            status_color = GREEN;
        } else if (strcmp(status, "pending") == 0) {
            status_color = YELLOW;
        } else if (strcmp(status, "failed") == 0) {
            status_color = RED;
        } else if (strcmp(status, "cancelled") == 0) {
            status_color = RED;
        } else if (strcmp(status, "in_progress") == 0) {
            status_color = BLUE;
        }

        printf("│ %-6d │ %-19.19s │ %-27.27s │ %s%-8s" RESET " │\n", id, title,
               description, status_color, status);

    } while (sqlite3_step(stmt) == SQLITE_ROW);

    printf(CYAN BOLD "└────────┴─────────────────────┴─────────────────────"
                     "────────┴──────────┘\n" RESET);
}
