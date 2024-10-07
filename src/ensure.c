#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "ensure.h"

#ifdef which_run_error
    unsigned char big256[8] = {0};
#endif

void ensureFailed(int expr, char *assertion, unsigned char count, char *file, unsigned int line, const char *function) {
    
    #ifdef which_run_error
        if (!(big256[count/8] & (1<< (count%8)))) {
            big256[count/8] = big256[count/8] | (1<< (count%8));
            printf( "%s:%d %s: Assertion \x1b[1m\'%s\'\x1b[0m exits with \x1b[1m%d\x1b[0m\n", file, line, function, assertion, count);
        }
    #endif

    if (expr) return;
    
    fprintf(stderr, "\x1b[31m%s:%d %s: Assertion \x1b[1m\'%s\'\x1b[0m\x1b[31m does not hold\x1b[0m\n", file, line, function, assertion);
    exit(count);
}

void ensureFailedf(int expr, char *assertion, unsigned char count, char *file, unsigned int line, const char *function, const char *format, ...) {
    
    #ifdef which_run_error
        if (!(big256[count/8] & (1<< (count%8)))) {
            big256[count/8] = big256[count/8] | (1<< (count%8));
            printf( "%s:%d %s: Assertion \x1b[1m\'%s\'\x1b[0m exits with \x1b[1m%d\x1b[0m\n", file, line, function, assertion, count);
        }
    #endif

    if (expr) return;
    
    fprintf(stderr, "\x1b[31m%s:%d %s: Assertion \x1b[1m\'%s\'\x1b[0m\x1b[31m does not hold\x1b[0m\n", file, line, function, assertion);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(count);
}
