#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ensure.h"

INCREMENT_COUNT(0, 4, 2);

#define REF_DIR "ref"
#define OUT_DIR "out"
#define BIN_DIR "bin"
#define IN_DIR "in"

/* Theminator
 *
 * Run code
 * get inputs from folder
 * put ouput into folder
 *
 */
char compiler[100] = "gcc";
char compilerFlags[100] = "-std=c99 -pedantic -Wall";
int main(int argc, char const* argv[]) {
    printf("hi\n");
    printf("hi");
    delay(1000);
    printf("\33[2K\r");
    // printf("bye\n");
}

/*
 * Themulator
 * tltr -out folder -in folder -bin bin -case folder folder
 * valgrind debu
 *
 * https://codereview.stackexchange.com/questions/139440/loading-animating-dots-in-c
 * [x] wrong
 * [.] [-] [+] [o] [ ] running
 * [/] [|] [\] [-]
 * [?] not found
 * [v] valgrind
 * [/]
 *
 */