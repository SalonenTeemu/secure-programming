/*
 * This program reads a file, filters out all characters except letters, numbers,
 * commas, and hyphens, and prints the filtered result to the screen.
 * The program tries to prevent attacks by filtering out all unwanted characters.
 * Still, encodings such as base64 or hex could bypass the filter and
 * if the data was used later somewhere, it could lead to security issues.
 *
 * Compile with: gcc -o t4 t4.c
 */

#include <ctype.h>
#include <stdio.h>

// Function to filter the content of a file and print the filtered result
void filterAndPrintFile(const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the given file in read mode

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char ch;
    printf("Filtered content:\n");

    // Read each character from the file until the end (EOF)
    while ((ch = fgetc(file)) != EOF) {
        // Check if the character is a letter, digit, comma, or hyphen
        if (isalnum(ch) || ch == ',' || ch == '-') {
            putchar(ch);
        }
    }

    fclose(file);
    printf("\n");
}

int main() {
    char filename[100];

    // Ask the user for the filename
    printf("Enter the filename to read and filter: ");
    scanf("%99s", filename);

    // Call the function to filter and print the file content
    filterAndPrintFile(filename);

    return 0;
}
