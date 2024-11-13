#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[1024];  // Buffer to store each line of input

    while (fgets(buffer, sizeof(buffer), stdin)) {
        system(buffer);
    }

    return 0;
}
