#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char const* argv[]) {

    int n ;
    scanf("%d\n", &n);
    if (n == -1) return 1;

    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        printf("%d\n", a + b);
    }
    return 0;
}