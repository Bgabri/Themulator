#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "parseArgs.h"
#include "themis.h"
#include "https.h"
#include "json.h"

#define MAX_CMD_LEN 4096

typedef struct dirent dirent;

typedef struct nibbler {
    unsigned int _1 : 8, _2 : 8, _3 : 8, _4 : 8;
} nibbler;

Options options = {0};


/*
 * checks if the given string ends with the given suffix 
 */
int endsWith(char *str, char *suffix) {
    size_t strLength = strlen(str);
    size_t suffixLength = strlen(suffix);
    if (strLength < suffixLength) return 0;

    return strcmp(str + strLength - suffixLength, suffix) == 0;
}

/*
 * replace the end of str0 with str1 starting at pos
 * if pos < 0 then pos = length(str0) + pos
 */
char *replace(char *str0, char *str1, int pos) {
    size_t str0Length = strlen(str0);
    size_t str1Length = strlen(str1);
    if (pos < 0) pos = str0Length + pos;

    size_t newLength = pos + str1Length;
    if (newLength < str0Length) newLength = str0Length;

    char *strNew = calloc(newLength + 1, sizeof(char));
    strcpy(strNew, str0);
    strcpy(strNew + pos, str1);
    return strNew;
}

/*
 * runs a command 
 * returns the exit code of the command 
 */
int safeSystem(char *command) {
    if (options.verbose || options.dryRun) printf("%s\n", command);

    if (options.dryRun) return 0;
    int result = system(command);

    return WEXITSTATUS(result);
}

/*
 * creates the given directory 
 */
void createDir(char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) != -1) return;

    if (mkdir(dir, 0700) == 0) return;
    perror("Unable to create output directory");
}

/*
 * compiles the c program in the given directory
 * by default gcc *c -std=c99 -pedantic -Wall -o bin/program
 */
void compileProgram(char *dir, char *binDir, char *binName) {
    char command[MAX_CMD_LEN] = {0};

    char path[MAX_CMD_LEN] = {0};
    sprintf(path, "%s/%s", dir, binDir);
    createDir(path);
    sprintf(command, "%s %s/*c %s -o %s/%s/%s", options.compiler, dir, options.compilerFlags,
            dir, binDir, binName);

    int result = safeSystem(command);
    if (result == 0) return;

    printf("\x1b[35mCompilation error\x1b[0m\n");
    exit(0);
}

int sortEntries(const void *_a, const void *_b) {
    dirent** a = (dirent**) _a;
    dirent** b = (dirent**) _b;
    return strcmp((*a)->d_name, (*b)->d_name);
}

/*
 * returns all the file entries in the  given directory
 */
dirent **getEntires(DIR *dir, int *size) {
    if (dir == NULL) {
        perror("Unable to open input directory");
        return NULL;
    }

    char outPath[MAX_CMD_LEN] = {0};
    sprintf(outPath, "%s/%s", options.dir, options.outDir);
    createDir(outPath);

    int numEntries = 0;
    int allocSize = 2;
    dirent **entries = calloc(allocSize, sizeof(dirent *));

    dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (allocSize <= numEntries) {
            allocSize *= 2;
            entries = realloc(entries, allocSize*sizeof(dirent *));
        }
        entries[numEntries++] = entry;
    }
    qsort(entries, numEntries, sizeof(dirent *), sortEntries);

    *size = numEntries;

    return entries;
}

void runInput() {
    char inPath[MAX_CMD_LEN] = {0};
    sprintf(inPath, "%s/%s", options.dir, options.inDir);
    DIR *dir = opendir(inPath);

    int numEntries = 0;
    dirent **entries = getEntires(dir, &numEntries);

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

        sprintf(pipes, "< %s/%s/%s > %s/%s/%s 2> %s/%s/%s",
                options.dir, options.inDir, inFile,
                options.dir, options.outDir, outFile,
                options.dir, options.outDir, errFile);

        if (options.valgrind) {
            sprintf(command, "valgrind --leak-check=full -s ./%s/%s/%s %s", options.dir, options.binDir, options.binName, pipes);
        } else {
            sprintf(command, "./%s/%s/%s %s", options.dir, options.binDir, options.binName, pipes);
        }

        char cmprCommand[MAX_CMD_LEN] = {0};

        sprintf(cmprCommand, "cmp -s %s/%s/%s %s/%s/%s",
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

int main(int argc, char *argv[]) {
    // json();
    // return 0;
    options = parseOptions(argc, argv);

    compileProgram(options.dir, options.binDir, options.binName);
    runInput(options.inDir, options.outDir, options.refDir);
    return 0;
}
