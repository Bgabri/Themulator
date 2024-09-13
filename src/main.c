/*
 * [O] 1.in: 
 * []
 */





#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_CMD_LEN 1024

// #define DIR "Test"
#define REF_DIR "ref"
#define OUT_DIR "out"
#define BIN_DIR "bin"
#define IN_DIR "in"

char compiler[100] = "gcc";
char compilerFlags[100] = "-std=c99 -pedantic -Wall";

typedef struct Options {
    
    char *compiler;
    char *compilerFlags;

    short silent;
    short dryRun;

    char *dir;
    char *binDir;
    char *binName;
    char *inDir;
    char *outDir;
    char *refDir;

} Options;

Options options = {0};

int endsWith(char *str, char *suffix) {
    size_t strLength = strlen(str);
    size_t suffixLength = strlen(suffix);
    if (strLength < suffixLength) return 0;

    return strcmp(str + strLength - suffixLength, suffix) == 0;
}

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

void safeSystem(char *command) {
    if (!options.silent || options.dryRun) printf("%s\n", command);

    if (options.dryRun) return;
    int result = system(command);
    if (result != 0) perror("Error executing program");
}

void createDir(char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) != -1) return;

    if (mkdir(dir, 0700) == 0) return;
    perror("Unable to create output directory");
}


/*
 * gcc *c -std=c99 -pedantic -Wall -o dir/bin/name
 */
void compileProgram(char *dir, char *binDir, char *binName) {
    char command[MAX_CMD_LEN] = {0};

    char adf[MAX_CMD_LEN] = {0};
    sprintf(adf, "%s/%s", dir, binDir);
    createDir(adf);
    sprintf(command, "%s %s/*c %s -o %s/%s/%s", compiler, dir, compilerFlags,
                                               dir, binDir, binName);

    safeSystem(command);
}


void runInput(char *inDir, char *outDir, char *refDir) {

    struct dirent *entry;
    DIR *dir = opendir(inDir);

    // Check if the input directory was opened successfully
    if (dir == NULL) {
        perror("Unable to open input directory");
        return;
    }

    createDir(outDir);

    // Iterate over all files in the input folder
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".."

        char *inFile = entry->d_name;
        if (strcmp(inFile, ".") == 0 || strcmp(inFile, "..") == 0) continue;

        if (!endsWith(inFile, ".in")) continue;

        char *outFile = replace(inFile, "out", -2);

        char *errFile = replace(inFile, "err", -2);


        char command[MAX_CMD_LEN] = {0};
        
        sprintf(command, "./%s/%s/%s < %s/%s/%s > %s/%s/%s", options.dir, options.binDir, options.binName,
                                                    options.dir, options.inDir, inFile,
                                                    options.dir, options.outDir, outFile);

        safeSystem(command);
        free(outFile);
        free(errFile);
    }

    // Close the input directory
    closedir(dir);
}

int main(int argc, char *argv[]) {
    // if (argc != 3) {
    //     fprintf(stderr, "Usage: %s <input_folder> <output_folder>\n", argv[0]);
    //     return 1;
    // }
    options.dryRun = 0;
    options.silent = 0;

    options.dir = "Test"; 
    options.binDir = "bin";
    options.binName = "name";

    options.inDir = "in";
    options.outDir = "out";
    options.refDir = "ref";



    char input_folder[] = "Test/in";
    char output_folder[] = "Test/out";
    char refFolder[] = "Test/ref";

    compileProgram(options.dir, options.binDir, options.binName);
    runInput(input_folder, output_folder, refFolder);

    return 0;
}
