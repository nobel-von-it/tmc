#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"


void print_usage(int argc, char **argv) {
    printf("Usage:\n");
    printf("  %s add <title> <description>\n", argv[0]);
    printf("  %s delete <id>\n", argv[0]);
    printf("  %s edit <id> <title> <description>\n", argv[0]);
    printf("  %s view\n", argv[0]);
}

void call_error(sqlite3 *db, int argc, char **argv, const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    print_usage(argc, argv);
    sqlite3_close(db);
    exit(1);
}

int main(int argc, char **argv) {
    sqlite3 *db;
    int rc = sqlite3_open("tasks.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    create_if_not_exist(db);

    if (strcmp(argv[1], "add") == 0) {
        if (argc < 4) {
            call_error(db, argc, argv, "missing arguments for 'add' command");
        }
        add_task(db, argv[2], argv[3]);
    } else if (strcmp(argv[1], "delete") == 0) {
        if (argc < 3) {
            call_error(db, argc, argv,
                       "missing arguments for 'delete' command");
        }
        for (int i = 2; i < argc; i++) {
            delete_task(db, atoi(argv[i]));
        }
    } else if (strcmp(argv[1], "edit") == 0) {
        if (argc < 5) {
            call_error(db, argc, argv, "missing arguments for 'edit' command");
        }
        edit_task(db, atoi(argv[2]), argv[3], argv[4]);
    } else if (strcmp(argv[1], "view") == 0) {
        if (argc > 2) {
            call_error(db, argc, argv, "too many arguments for 'view' command");
        }
        view_tasks(db);
    } else {
        call_error(db, argc, argv, "unknown command");
    }

    sqlite3_close(db);
    return 0;
}
