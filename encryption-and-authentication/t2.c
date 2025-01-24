/**
 * This program generates cryptographically secure random data using OpenSSL's RAND_bytes()
 * and writes this random data to a binary file named "random_data.bin" or a text file named "random_data.txt".
 *
 * - https://docs.openssl.org/1.1.1/man3/RAND_bytes/
 *
 * Compile with: gcc -o t2 t2.c -lssl -lcrypto
 */

#include <openssl/rand.h>
#include <stdio.h>

#define DATA_SIZE 256  // Size of the random data to be generated

// Function to convert binary data to a hex string
void toHexString(const unsigned char* data, size_t length, char* output) {
    for (size_t i = 0; i < length; ++i) {
        sprintf(output + (i * 2), "%02x", data[i]);
    }
    output[length * 2] = '\0';
}

int main() {
    FILE* file;                         // Pointer to file
    unsigned char data[DATA_SIZE];      // Buffer to store random data
    int choice;                         // User's choice
    char hexOutput[DATA_SIZE * 2 + 1];  // Buffer to store hex string

    printf("1. Save to .bin file\n2. Save to .text file\nChoose an option: ");
    scanf("%d", &choice);
    getchar();

    // Generate cryptographically secure random data using OpenSSL's RAND_bytes function
    if (RAND_bytes(data, sizeof(data)) != 1) {
        fprintf(stderr, "Error generating random data\n");
        return 1;
    }

    if (choice == 1) {
        // Open file for writing in binary mode
        file = fopen("random_data.bin", "wb");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Write the generated random data to the file
        if (fwrite(data, 1, sizeof(data), file) != sizeof(data)) {
            perror("Error writing to file");
            fclose(file);
            return 1;
        }

        fclose(file);  // Close the file
        printf("Random data written to random_data.bin\n");
    } else if (choice == 2) {
        // Convert the random data to a hex string
        toHexString(data, sizeof(data), hexOutput);

        // Open file for writing in text mode
        file = fopen("random_data.txt", "w");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Write the hex string to the file
        if (fprintf(file, "%s", hexOutput) < 0) {
            perror("Error writing to file");
            fclose(file);
            return 1;
        }

        fclose(file);  // Close the file
        printf("Random data written to random_data.txt\n");
    } else {
        printf("Invalid choice\n");
        return 1;
    }

    return 0;
}
