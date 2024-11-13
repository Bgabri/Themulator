#include "io.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "parseArgs.h"

extern Options options;

void hideInput(int enable) {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);  // Get terminal attributes
    if (enable) {
        t.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    } else {
        t.c_lflag |= ICANON | ECHO;  // Re-enable canonical mode and echo
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &t);  // Set new terminal attributes
}

char *scanString(int *size, char endChar, char hide) {
    if (*size < 1) *size = 1;
    if (hide) hideInput(1);
    char current = getchar();

    char *p = calloc((*size + 1), sizeof(char));

    int iPosition = 0;
    while (current != endChar) {
        if (hide) {
            if (current == 127) {
                iPosition--;
                if (iPosition < 0) iPosition = 0;
                printf("\x1b[1D\x1b[0K");
            } else if (32 < current) {
                putchar(hide);
            }
        }
        if (*size <= iPosition) {
            *size *= 2;
            p = realloc(p, sizeof(char) * (*size + 1));
        }

        if (31 < current && current < 127) p[iPosition++] = current;

        current = getchar();
    }
    if (hide) {
        putchar(endChar);
        hideInput(0);
    }
    *size = iPosition;
    p[iPosition] = '\0';
    return p;
}

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
 * checks if the given string starts with the given prefix
 */
int startsWith(char *str, char *prefix) {
    size_t strLength = strlen(str);
    size_t prefixLength = strlen(prefix);
    if (strLength < prefixLength) return 0;

    for (int i = 0; i < prefixLength; i++) {
        if (str[i] != prefix[i]) return 0;
    }
    return 1;

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
int createPath(char *path) {
    if (pathExists(path)) return 0;

    if (mkdir(path, 0700) == 0) return 1;
    perror("Unable to create directory");
    return 0;
}

int removePath(char *path) {
    if (!pathExists(path)) return 0;
    if(rmdir(path) == 0) return 1;

    perror("Unable to remove directory");
    return 0;
}

int pathExists(char *path) {
    struct stat st = {0};
    return stat(path, &st) != -1;
}

int sortEntries(const void *_a, const void *_b) {
    dirent **a = (dirent **)_a;
    dirent **b = (dirent **)_b;
    return strcmp((*a)->d_name, (*b)->d_name);
}

/*
 * returns all the file entries in the  given directory
 */
dirent **getEntries(DIR *dir, int *size) {
    if (dir == NULL) {
        perror("Unable to open input directory");
        return NULL;
    }

    char outPath[MAX_CMD_LEN] = {0};
    sprintf(outPath, "%s/%s", options.dir, options.outDir);
    createPath(outPath);

    int numEntries = 0;
    int allocSize = 2;
    dirent **entries = calloc(allocSize, sizeof(dirent *));

    dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (allocSize <= numEntries) {
            allocSize *= 2;
            entries = realloc(entries, allocSize * sizeof(dirent *));
        }
        entries[numEntries++] = entry;
    }
    qsort(entries, numEntries, sizeof(dirent *), sortEntries);

    *size = numEntries;

    return entries;
}
