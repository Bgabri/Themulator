#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ensure.h"

INCREMENT_COUNT(0, 4, 2);

int main(int argc, char const* argv[]) {

    ensure(1+1 == 2);
    
    char input[100];
    scanf("%s", input);
    printf("%s\n", input);

    
    return 0;
}