#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ensure.h"

INCREMENT_COUNT(0, 4, 2);

int main(int argc, char const* argv[]) {

    int n ;
    scanf("%d\n", &n);

    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        printf("%d\n", a+b);
    }
    
    return 0;
}