#include "config.h"

#include <stdio.h>

#include "io.h"
#include "ensure.h"

void parseLine(FILE *file, Options *option) {

    int size = 0;
    char *key = fscanString(file, &size, ':');
    char c;
    while ((c = fgetc(file) != EOF)) {
        if (c == '\"') break;
    }
    char *value = fscanString(file, &size, '\"');
    while ((c = fgetc(file) != EOF)) {
        if (c == '\n') break;
    }
    printf("%s : %s", key, value);
}

Options parseConfig(char *path) {
    Options o = {0};
    FILE *file;

    file = fopen(path, "r");
    ensure(file != NULL);

    fclose(file);
    return  o;
}
