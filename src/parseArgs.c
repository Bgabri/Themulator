#include "parseArgs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Options options = {0};

int parseCompiler(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->compiler = argv[*i];
    (*i)++;

    // remove default flags from options if compiler is set
    if (strcmp(o->compilerFlags, _COMPILER_FLAGS) == 0) {
        o->compilerFlags = ""; // TODO: ?!
    }
    return 1;
}

int parseCompilerFlags(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->compilerFlags = argv[*i];
    (*i)++;
    return 1;
}

int parseVerbose(char *argv[], int *i, Options *o) {
    o->verbose = 1;
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
    if (argv[*i] == NULL) return -1;
    o->dir = argv[*i];
    (*i)++;
    return 1;
}

int parseBinDir(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->binDir = argv[*i];
    (*i)++;
    return 1;
}

int parseBinName(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->binName = argv[*i];
    (*i)++;
    return 1;
}

int parseInDir(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->inDir = argv[*i];
    (*i)++;
    return 1;
}

int parseOutDir(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->outDir = argv[*i];
    (*i)++;
    return 1;
}

int parseRefDir(char *argv[], int *i, Options *o) {
    if (argv[*i] == NULL) return -1;
    o->refDir = argv[*i];
    (*i)++;
    return 1;
}

int parseCommand(char *argv[], int *i, Options *o) {
    if (argv == NULL) return -1;

    char *arg = argv[*i];
    (*i)++;
    if (!strcmp(arg, "run")) o->command = run;
    else if (!strcmp(arg, "compile")) o->command = compile;
    else if (!strcmp(arg, "download")) o->command = download;
    else if (!strcmp(arg, "interpret")) o->command = interpret;
    else return -1;
    return 1;
}

int printHelp(char *argv[], int *i, Options *o) {
    printf("Usage: thml [options] <command>\n");
    printf("Commands:\n");
    printf("\tdownload\n");
    printf("\t\tStarts an interface to login and download files from Themis.\n");
    printf("\trun\n");
    printf("\t\tCompiles and runs the program with the given option.\n");
    printf("\tinterpret\n");
    printf("\t\tinterpret the program with the given option.\n");
    printf("Options:\n");
    printf("\t--help, -h\n");
    printf("\t\tDisplay this information\n");
    printf("\t--compiler, -c\n");
    printf("\t\tThe compiler or interpreter to use\n");
    printf("\t--compiler-flags, -f\n");
    printf("\t\tThe flags to pass to the compiler\n");
    printf("\t--dry-run, -d\n");
    printf("\t\trun without command execution\n");
    printf("\t--verbose\n");
    printf("\t\tprint out all the execution steps\n");
    printf("\t--valgrind, -v\n");
    printf("\t\truns the binary through valgrind\n");
    printf("\t--bin-name <name>, -b <name>\n");
    printf("\t\tthe name to give to the outputted binary\n");
    printf("\t--dir <folder>, -D <folder>\n");
    printf("\t\tthe folder of the source files to judge\n");
    printf("\t--in-dir, -I <folder>\n");
    printf("\t\tthe folder to the input of the test cases\n");
    printf("\t--out-dir <folder>, -O <folder>\n");
    printf("\t\tthe folder where the output is placed\n");
    printf("\t--bin-dir <folder>, -B <folder>\n");
    printf("\t\tthe folder to output the binaries to\n");
    printf("\t--ref-dir <folder>, -R <folder>\n");
    printf("\t\tthe folder with the reference outputs of the test cases\n");
    exit(0);
    return -1;
}

int parseDouble(char *argv[], int *i, Options *o) {
    char *arg = argv[*i];
    (*i)++;

    if (!strcmp(arg, "--compiler")) return parseCompiler(argv, i, o);
    if (!strcmp(arg, "--compiler-flags")) return parseCompilerFlags(argv, i, o);
    if (!strcmp(arg, "--dry-run")) return parseDryRun(argv, i, o);
    if (!strcmp(arg, "--verbose")) return parseVerbose(argv, i, o);
    if (!strcmp(arg, "--valgrind")) return parseValgrind(argv, i, o);
    if (!strcmp(arg, "--bin-name")) return parseBinName(argv, i, o);
    if (!strcmp(arg, "--help")) return printHelp(argv, i, o);
    if (!strcmp(arg, "--bin-dir")) return parseBinDir(argv, i, o);
    if (!strcmp(arg, "--dir")) return parseDir(argv, i, o);
    if (!strcmp(arg, "--in-dir")) return parseInDir(argv, i, o);
    if (!strcmp(arg, "--out-dir")) return parseOutDir(argv, i, o);
    if (!strcmp(arg, "--ref-dir")) return parseRefDir(argv, i, o);

    return -1;
}

int parseOption(char *argv[], int *i, Options *o) {
    if (argv == NULL) return -1;
    if (argv[*i][0] != '-') return parseCommand(argv, i, o);

    char c = argv[*i][1];
    (*i)++;
    switch (c) {
        case 'c':
            return parseCompiler(argv, i, o);
        case 'f':
            return parseCompilerFlags(argv, i, o);
        case 'd':
            return parseDryRun(argv, i, o);
        case 'v':
            return parseValgrind(argv, i, o);
        case 'b':
            return parseBinName(argv, i, o);
        case 'h':
            return printHelp(argv, i, o);
        case 'B':
            return parseBinDir(argv, i, o);
        case 'D':
            return parseDir(argv, i, o);
        case 'I':
            return parseInDir(argv, i, o);
        case 'O':
            return parseOutDir(argv, i, o);
        case 'R':
            return parseRefDir(argv, i, o);
        case '-':
            --(*i);
            return parseDouble(argv, i, o);
        default:
            return -1;
    }
}

Options parseOptions(int argc, char *argv[]) {
    options.compiler = _COMPILER;
    options.compilerFlags = _COMPILER_FLAGS;

    options.verbose = _VERBOSE;
    options.dryRun = _DRYRUN;
    options.valgrind = _VALGRIND;

    options.dir = _DIR;
    options.binDir = _BIN_DIR;
    options.binName = _BIN_NAME;
    options.inDir = _IN_DIR;
    options.outDir = _OUT_DIR;
    options.refDir = _REF_DIR;

    options.command = _COMMAND;

    int i = 1;
    while (argv[i] != NULL) {
        int result = parseOption(argv, &i, &options);
        if (result == -1) {
            printf("Invalid usage, use --help for usage\n");
            exit(0);
        }
    }
    return options;
}

void printOptions(Options options) {
    printf("compiler:      %s\n", options.compiler);
    printf("compilerFlags: %s\n", options.compilerFlags);
    printf("verbose:       %d\n", options.verbose);
    printf("dryRun:        %d\n", options.dryRun);
    printf("valgrind:      %d\n", options.dryRun);
    printf("dir:           %s\n", options.dir);
    printf("binDir:        %s\n", options.binDir);
    printf("binName:       %s\n", options.binName);
    printf("inDir:         %s\n", options.inDir);
    printf("outDir:        %s\n", options.outDir);
    printf("refDir:        %s\n", options.refDir);
}
