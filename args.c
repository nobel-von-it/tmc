#include "args.h"
#include <stdio.h>

void print_usage(void) {
    printf("Usage: ./tm <command> [options]\n");
    printf("Commands:\n");
    printf("  add      -t <title> -d <description> [-p <path>] [-v]\n");
    printf("  delete   -i <id> [-p <path>] [-v]\n");
    printf("  edit     -i <id> -t <title> -d <description> [-p <path>] [-v]\n");
    printf("  mark     -i <id> -s <status> [-p <path>] [-v]\n");
    printf("  view     [-p <path>] [-v]\n");
    printf("Options:\n");
    printf("  -p, --path <path>    Specify storage directory\n");
    printf("  -v, --verbose        Enable detailed output\n");
}

void call_error(sqlite3 *db, const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    print_usage();
    sqlite3_close(db);
    exit(1);
}

void default_arg(Arg *arg) {
    arg->command = NONE;
    arg->id = -1;
    arg->title = NULL;
    arg->description = NULL;
    arg->path = NULL;
    arg->verbose = false;
}

bool parse_args(int argc, char **argv, Arg *arg) {
    default_arg(arg);

    if (argc < 2) {
        return false;
    }

    if (strcmp(argv[1], "add") == 0) {
        arg->command = ADD;
    } else if (strcmp(argv[1], "delete") == 0) {
        arg->command = DELETE;
    } else if (strcmp(argv[1], "edit") == 0) {
        arg->command = EDIT;
    } else if (strcmp(argv[1], "view") == 0) {
        arg->command = VIEW;
    } else if (strcmp(argv[1], "mark") == 0) {
        arg->command = MARK;
    } else {
        return false;
    }

    for (int i = 2; i < argc; ++i) {
        if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--title") == 0) &&
            i + 1 < argc) {
            arg->title = argv[++i];
        } else if ((strcmp(argv[i], "-d") == 0 ||
                    strcmp(argv[i], "--description") == 0) &&
                   i + 1 < argc) {
            arg->description = argv[++i];
        } else if ((strcmp(argv[i], "-i") == 0 ||
                    strcmp(argv[i], "--id") == 0) &&
                   i + 1 < argc) {
            arg->id = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-p") == 0 ||
                    strcmp(argv[i], "--path") == 0) &&
                   i + 1 < argc) {
            arg->path = argv[++i];
        } else if (strcmp(argv[i], "-v") == 0 ||
                   strcmp(argv[i], "--verbose") == 0) {
            arg->verbose = true;
        } else if ((strcmp(argv[i], "-s") == 0 ||
                    strcmp(argv[i], "--status") == 0) &&
                   i + 1 < argc) {
            arg->status = argv[++i];
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            return false;
        }
    }

    if (arg->command == ADD && arg->title == NULL) {
        fprintf(stderr, "Missing title for 'add' command\n");
        return false;
    } else if (arg->command == DELETE && arg->id == -1) {
        fprintf(stderr, "Missing ID for 'delete' command\n");
        return false;
    } else if (arg->command == EDIT && (arg->id == -1 || arg->title == NULL)) {
        fprintf(stderr, "Missing ID or title for 'edit' command\n");
        return false;
    } else if (arg->command == MARK && (arg->id == -1 || arg->status == NULL)) {
        fprintf(stderr, "Missing ID or status for 'mark' command\n");
        return false;
    }
    // TODO: add path checker

    return true;
}
