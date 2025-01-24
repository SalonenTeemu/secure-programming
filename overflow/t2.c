/**
 * This program demonstrates an off-by-one buffer overflow.
 * A buffer of size 8 bytes is created, but an attempt is made to write to buffer[8],
 * which is one index beyond the allocated space for the buffer.
 */

#include <stdio.h>
#include <string.h>

int main() {
    char buffer[8];  // Buffer of 8 bytes
    int i;

    printf("Starting off-by-one buffer overflow...\n");

    // Incorrect loop that writes to buffer[8] (which is out of bounds)
    for (i = 0; i <= 8; i++) {
        buffer[i] = 'A';
        printf("Writing 'A' to buffer[%d] at address %p\n", i, (void*)&buffer[i]);  //
    }

    return 0;
}
