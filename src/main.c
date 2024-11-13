#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseArgs.h"
#include "themis.h"
#include "io.h"

extern Options options;

typedef struct nibbler {
    unsigned int _1 : 8, _2 : 8, _3 : 8, _4 : 8;
} nibbler;

/*
 * compiles the c program in the given directory
 * by default gcc *c -std=c99 -pedantic -Wall -o bin/program
 */
void compileProgram(char *dir, char *binDir, char *binName) {
    char command[MAX_CMD_LEN] = {0};

    char path[MAX_CMD_LEN] = {0};
    sprintf(path, "%s/%s", dir, binDir);
    int created = createPath(path);
    sprintf(command, "%s \"%s/\"*c %s -o \"%s/%s/%s\"", options.compiler, dir, options.compilerFlags,
            dir, binDir, binName);

    int result = safeSystem(command);
    if (result == 0) return;

    if (created) {
        removePath(path);
    }

    printf("\x1b[35mCompilation error\x1b[0m\n");
    exit(0);
}

void runInput() {
    char inPath[MAX_CMD_LEN] = {0};
    sprintf(inPath, "%s/%s", options.dir, options.inDir);
    DIR *dir = opendir(inPath);

    int numEntries = 0;
    dirent **entries = getEntries(dir, &numEntries);

    for (int i = 0; i < numEntries; i++) {
        dirent *entry = entries[i];
        char *inFile = entry->d_name;

        if (strcmp(inFile, ".") == 0 || strcmp(inFile, "..") == 0) continue;
        if (!endsWith(inFile, ".in")) continue;

        char *outFile = replace(inFile, "out", -2);

        char *errFile = replace(inFile, "err", -2);

        // merge and build command
        char command[MAX_CMD_LEN * 2] = {0};

        char pipes[MAX_CMD_LEN] = {0};

        sprintf(pipes, "< \"%s/%s/%s\" > \"%s/%s/%s\" 2> \"%s/%s/%s\"",
                options.dir, options.inDir, inFile,
                options.dir, options.outDir, outFile,
                options.dir, options.outDir, errFile);

        if (options.valgrind) {
            sprintf(command, "valgrind --leak-check=full -s \"%s/%s/%s\" %s", options.dir, options.binDir, options.binName, pipes);
        } else {
            sprintf(command, "\"%s/%s/%s\" %s", options.dir, options.binDir, options.binName, pipes);
        }

        if (options.command == interpret) {
            sprintf(command, " %s %s %s", options.compiler, options.compilerFlags, pipes);
        }

        char cmprCommand[MAX_CMD_LEN] = {0};

        sprintf(cmprCommand, "cmp -s \"%s/%s/%s\" \"%s/%s/%s\"",
                options.dir, options.outDir, outFile,
                options.dir, options.refDir, outFile);

        // run commands
        int runResult = safeSystem(command);
        int cmprResult = safeSystem(cmprCommand);

        // print the results
        if (runResult == 0 && cmprResult == 0) {
            printf("\x1b[32m\x1b[1m[o]\x1b[0m %s\n", inFile);
        } else if (runResult != 0) {
            printf("\x1b[35m\x1b[1m[e]\x1b[0m %s", inFile);
            printf("\t\x1b[2mRuntime error: Program exited with code: %d\x1b[0m\n", runResult);
        } else if (cmprResult == 1) {
            printf("\x1b[31m\x1b[1m[x]\x1b[0m %s", inFile);
            printf("\t\x1b[2mWrong output\x1b[0m\n");
        } else {
            printf("\x1b[33m\x1b[1m[?]\x1b[0m %s", inFile);
            printf("\t\x1b[2mReference file not found.\x1b[0m\n");
        }

        free(outFile);
        free(errFile);
    }

    closedir(dir);
    free(entries);
}


char* setupConfig() {
    // https://specifications.freedesktop.org/basedir-spec/latest
    char *thmlConfigPath = calloc(MAX_CMD_LEN, sizeof(char));

    char configsPath[MAX_CMD_LEN] = {0};
    if (getenv("XDG_DATA_HOME") != NULL) {
        strcpy(configsPath, getenv("XDG_DATA_HOME"));
    } else {
        sprintf(configsPath, "%s/.local/share", getenv("HOME"));
    }

    if (!pathExists(configsPath)) {
        fprintf(stderr, "Config setup: failed, %s not found\n", configsPath);
        exit(1);
    }

    sprintf(thmlConfigPath, "%s/themulator", configsPath);
    createPath(thmlConfigPath);
    return thmlConfigPath;
}

int main(int argc, char *argv[]) {
    char *thmlConfigPath = setupConfig();

    parseOptions(argc, argv);

    char cookieDir[MAX_CMD_LEN] = {0};
    sprintf(cookieDir, "%s/cookie.jar", thmlConfigPath);

    switch (options.command) {
        case run:
            compileProgram(options.dir, options.binDir, options.binName);
            runInput();
            break;
        case compile:
            compileProgram(options.dir, options.binDir, options.binName);
            break;
        case interpret:
            runInput();
            break;
        case download:
            themis(cookieDir);
            break;
    }

    free(thmlConfigPath);
    return 0;
}
