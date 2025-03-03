#include <sqlite3.h>
#include <stdio.h>

#include "args.h"
#include "db.h"

int main(int argc, char **argv) {
    Arg arg;
    sqlite3 *db;

    int rc = sqlite3_open("tasks.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    if (!parse_args(argc, argv, &arg)) {
        call_error(db, "Invalid arguments");
    }

    create_if_not_exist(db);

    switch (arg.command) {
    case ADD:
        add_task(db, &arg);
        break;
    case DELETE:
        delete_task(db, &arg);
        break;
    case EDIT:
        edit_task(db, &arg);
        break;
    case VIEW:
        view_tasks(db);
        break;
    default:
        break;
    }

    sqlite3_close(db);
    return 0;
}
