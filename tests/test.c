#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[1024];  // Buffer to store each line of input

    while (fgets(buffer, sizeof(buffer), stdin)) {
        int result = system(buffer);
        if (result != 0) {
            exit(result);
        };
    }

    return 0;
}
