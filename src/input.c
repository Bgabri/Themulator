#include "input.h"

#include <stdio.h>

/*
 * -c --compiler
 * -f --compilerFlags
 * -o --
 * -i
 * -r
 * -s
 *
 */

int parseCompiler(char *argv, Options o) {
    printf("parseCompiler is not implemented\n");
    exit(0);
    return -1;
}

int parseCompilerFlags(char *argv, Options o) {
    printf("parseCompilerFlags is not implemented\n");
    exit(0);
    return -1;
}

int parseSilent(char *argv, Options o) {
    printf("parseSilent is not implemented\n");
    exit(0);
    return -1;
}

int parseDryRun(char *argv, Options o) {
    printf("parseDryRun is not implemented\n");
    exit(0);
    return -1;
}

int parseDir(char *argv, Options o) {
    printf("parseDir is not implemented\n");
    exit(0);
    return -1;
}

int parseBinDir(char *argv, Options o) {
    printf("parseBinDir is not implemented\n");
    exit(0);
    return -1;
}

int parseBinName(char *argv, Options o) {
    printf("parseBinName is not implemented\n");
    exit(0);
    return -1;
}

int parseInDir(char *argv, Options o) {
    printf("parseInDir is not implemented\n");
    exit(0);
    return -1;
}

int parseOutDir(char *argv, Options o) {
    printf("parseOutDir is not implemented\n");
    exit(0);
    return -1;
}

int parseRefDir(char *argv, Options o) {
    printf("parseRefDir is not implemented\n");
    exit(0);
    return -1;
}

int parseDouble(char *argv, Options o) {
    return -1;
}

int parseOption(char *argv[], Options o) {
    if (argv == NULL) return -1;
    if (argv[0][0] != '-') return -1;

    switch (argv[0][1]) {
        case 'c':
            return parseCompiler(argv+1, o);
        case 'f':
            return parseCompilerFlags(argv+1, o);
        case 's':
            return parseSilent(argv+1, o);
        case 'd':
            return parseDryRun(argv+1, o);
        case 'b': 
            return parseBinName(argv+1, o);

        case 'B':
            return parseBinDir(argv+1, o);
        case 'D':
            return parseDir(argv+1, o);
        case 'I':
            return parseInDir(argv+1, o);
        case 'O':
            return parseOutDir(argv+1, o);
        case 'R':
            return parseRefDir(argv+1, o);
        case '-':
            break;
        default:
            return -1;
    }
}

Options parseOptions(int argc, char *argv[]) {
    Options options = {0};

    for (int i = 1; i < argc; i += 2) {
        // char *arg = argv[i];
        int result = parseOption(argv + i, options);
        if (result == -1) {
            printf("Invalid usage\n");
            exit(0);
        }
    }
    return options;
}