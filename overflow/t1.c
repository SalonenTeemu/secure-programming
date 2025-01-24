/**
 * This program demonstrates buffer overflow.
 * A buffer of 8 bytes is created, but an infinite loop writes to this buffer
 * without checking the bounds, causing the buffer to overflow until the program crashes.
 */

#include <stdio.h>
#include <string.h>

int main() {
    char buffer[8];  // Buffer of 8 bytes
    int i = 0;

    printf("Starting buffer overflow...\n");

    // Infinite loop to write beyond the buffer's boundary
    while (1) {
        // Write 'A' to the buffer at position i
        buffer[i++] = 'A';

        // Print the index and address being written to
        printf("Writing 'A' to buffer[%d] at address %p\n", i - 1, (void*)&buffer[i - 1]);
    }

    printf("End of program\n");  // This line will never be reached
    
    return 0;
}
