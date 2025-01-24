/**
 * This program demonstrates a heap overflow by using two char* buffers
 * and writing beyond the allocated size of the first buffer to corrupt the second one.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *buffer1, *buffer2;

    // Allocate 8 bytes for the first buffer
    buffer1 = (char *)malloc(8 * sizeof(char));
    if (buffer1 == NULL) {
        printf("Memory allocation failed for buffer1\n");
        return 1;
    }

    // Allocate 8 bytes for the second buffer
    buffer2 = (char *)malloc(8 * sizeof(char));
    if (buffer2 == NULL) {
        printf("Memory allocation failed for buffer2\n");
        free(buffer1);
        return 1;
    }

    // Initialize buffer2 with some data
    strcpy(buffer2, "Initial");

    printf("Before overflow:\n");
    printf("buffer2: %s\n", buffer2);

    // Fill buffer1 with more than 8 characters to cause overflow
    strcpy(buffer1, "This will overflow buffer1 and corrupt buffer2!");

    printf("\nAfter overflow:\n");
    printf("buffer2: %s\n", buffer2);  // This should print corrupted data if overflowed

    // Free allocated memory
    free(buffer1);
    free(buffer2);

    return 0;
}
