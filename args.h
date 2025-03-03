#ifndef ARGS_H
#define ARGS_H

#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.1"
#define AUTHOR "Nerd"

void print_usage(void);
void call_error(sqlite3 *db, const char *message);

typedef enum {
    ADD,
    DELETE,
    EDIT,
    VIEW,
    NONE,
} Command;

typedef struct {
    Command command;
    int id;
    char *title;
    char *description;
    char *path;
    bool verbose;
} Arg;

void default_arg(Arg *arg);
bool parse_args(int argc, char **argv, Arg *arg);

#endif
