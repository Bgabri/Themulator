#ifndef IO_H
#define IO_H

#include <dirent.h>

typedef struct dirent dirent;
#define MAX_CMD_LEN 4096


void hideInput(int enable);
char *scanString(int *size, char endChar, char hide);
int endsWith(char *str, char *suffix);
int startsWith(char *str, char *prefix);
char *replace(char *str0, char *str1, int pos);
int safeSystem(char *command);
int createDir(char *dir);
int removeDir(char *dir);
dirent **getEntries(DIR *dir, int *size);
#endif
