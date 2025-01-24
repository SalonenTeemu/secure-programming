/**
 * This program demonstrates a format string vulnerability in the printf function.
 * The user input is directly passed to printf without proper format specifiers.
 * This can lead to information disclosure from the stack.
 * Test with:
 *    - ./t5 "Hello world!" (normal usage)
 *    - ./t5 "%x %x %x %x %x %x %x %x %x %x" (format string vulnerability)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vulnerable function that improperly uses user input in printf
int vulnerableFunction(char buffer[256]) {
    int nReturn = 0;
    // User input is directly used as the format string
    printf(buffer);
    return nReturn;
}

int main(int argc, char *argv[]) {
    char buffer[256] = "";  // Allocate buffer
    if (argc == 2) {
        strncpy(buffer, argv[1], 255);  // Copy command line argument to buffer
    }
    vulnerableFunction(buffer);  // Pass buffer to the vulnerable function
    return 0;
}
