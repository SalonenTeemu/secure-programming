/*
 * This program demonstrates a function pointer overflow.
 * The buffer overflows into a function pointer, causing the program
 * to call an unintended (malicious) function after the overflow.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void safeFunction() {
    printf("This is the correct function.\n");
}

void unsafeFunction() {
    printf("This is the malicious function.\n");
}

void t4(char *input) {
    char buffer[16];                   // Small buffer to demonstrate overflow
    void (*funcPtr)() = safeFunction;  // Function pointer initially pointing to safeFunction

    // Call the safe function before any overflow
    printf("Before overflow: ");
    funcPtr();

    // Buffer overflow: copying more data than buffer can hold
    strcpy(buffer, input);  // This overflows and can potentially overwrite funcPtr

    // Call the function pointer after the overflow
    printf("After overflow: ");
    funcPtr();  // This might call unsafeFunction instead
}

int main() {
    char input[32];

    // Fill the input with 'A's, to overflow the buffer
    memset(input, 'A', 16);  // First 16 bytes for buffer overflow

    // Overwrite the function pointer with the address of unsafeFunction
    void (*maliciousFuncPtr)() = unsafeFunction;

    // Copy the address of unsafeFunction to the location after the buffer
    memcpy(input + 16, &maliciousFuncPtr, sizeof(maliciousFuncPtr));

    // Pass the crafted input to the t4 function
    t4(input);

    return 0;
}
