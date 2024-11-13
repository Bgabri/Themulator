#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char const* argv[]) {

    int n;
    int result = scanf("%d\n", &n);
    if (result != 1) return 1; 
    if (n == -1) return 1;

    for (int i = 0; i < n; i++) {
        int a, b;
        int result = scanf("%d %d", &a, &b);
        if (result != 2) return 1;
        printf("%d\n", a + b);
    }
    return 0;
}