
#include "input.h"

#include <stdio.h>
#include <stdlib.h>


int parseCompiler(char *argv[], int *i, Options *o) {
    printf("parseCompiler is not implemented\n");
    exit(0);
    return -1;
}

int parseCompilerFlags(char *argv[], int *i, Options *o) {
    printf("parseCompilerFlags is not implemented\n");
    exit(0);
    return -1;
}

int parseSilent(char *argv[], int *i, Options *o) {
    o->silent = 1;
    return 1;
}

int parseDryRun(char *argv[], int *i, Options *o) {
    o->dryRun = 1;
    return 1;
}

int parseValgrind(char *argv[], int *i, Options *o) {
    o->valgrind = 1;
    return 1;
}

int parseDir(char *argv[], int *i, Options *o) {
    o->dir = argv[*i];
    (*i)++;
    return 1;
}

int parseBinDir(char *argv[], int *i, Options *o) {
    o->binDir = argv[*i];
    (*i)++;
    return 1;
}

int parseBinName(char *argv[], int *i, Options *o) {
    o->binName = argv[*i];
    (*i)++;
    return 1;
}

int parseInDir(char *argv[], int *i, Options *o) {
    o->inDir = argv[*i];
    (*i)++;
    return 1;
}

int parseOutDir(char *argv[], int *i, Options *o) {
    o->outDir = argv[*i];
    (*i)++;
    return 1;
}

int parseRefDir(char *argv[], int *i, Options *o) {
    o->refDir = argv[*i];
    (*i)++;
    return 1;
}

int parseDouble(char *argv[], int *i, Options *o) {
    printf("parseRefDir is not implemented\n");
    exit(0);
    return -1;
}

int parseOption(char *argv[], int *i, Options *o) {
    if (argv == NULL) return -1;
    if (argv[*i][0] != '-') return -1;

    switch (argv[*i][1]) {
        case 'c': (*i)++;
            return parseCompiler(argv, i, o);
        case 'f': (*i)++;
            return parseCompilerFlags(argv, i, o);
            
        case 's': (*i)++;
            return parseSilent(argv, i, o);
        case 'd': (*i)++;
            return parseDryRun(argv, i, o);
        case 'v': (*i)++;
            return parseValgrind(argv, i, o);
        case 'b': (*i)++;
            return parseBinName(argv, i, o);

        case 'B': (*i)++;
            return parseBinDir(argv, i, o);
        case 'D': (*i)++;
            return parseDir(argv, i, o);
        case 'I': (*i)++;
            return parseInDir(argv, i, o);
        case 'O': (*i)++;
            return parseOutDir(argv, i, o);
        case 'R': (*i)++;
            return parseRefDir(argv, i, o);
        case '-':
            return parseDouble(argv, i, o);
        default:
            return -1;
    }
}

Options parseOptions(int argc, char *argv[]) {
    Options options = {0};
        options.compiler = _COMPILER;
        options.compilerFlags = _COMPILER_FLAGS;

        options.silent = _SILENT;
        options.dryRun = _DRYRUN;
        options.valgrind = _VALGRIND;

        options.dir = _DIR;
        options.binDir = _BIN_DIR;
        options.binName = _BIN_NAME;
        options.inDir = _IN_DIR;
        options.outDir = _OUT_DIR;
        options.refDir = _REF_DIR;

    int i = 1;
    while (argv[i] != NULL) {
        int result = parseOption(argv, &i, &options);
        if (result == -1) {
            printf("Invalid usage\n");
            exit(0);
        }
    }
    return options;
}

void printOptions(Options options) {

    printf("compiler:      %s\n", options.compiler);
    printf("compilerFlags: %s\n", options.compilerFlags);
    printf("silent:        %d\n", options.silent);
    printf("dryRun:        %d\n", options.dryRun);
    printf("valgrind:      %d\n", options.dryRun);
    printf("dir:           %s\n", options.dir);
    printf("binDir:        %s\n", options.binDir);
    printf("binName:       %s\n", options.binName);
    printf("inDir:         %s\n", options.inDir);
    printf("outDir:        %s\n", options.outDir);
    printf("refDir:        %s\n", options.refDir);

}