/**
 * This program implements file encryption and decryption using Advanced Encryption Standard with a 256-bit key.
 * Only authenticated users can encrypt or decrypt files.
 * The user's password is securely converted into an encryption key using PBKDF2.
 *
 * - https://docs.openssl.org/3.0/man3/PKCS5_PBKDF2_HMAC/
 *
 * Compile with: gcc -o t4 t4.c -lssl -lcrypto
 */

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SALT_SIZE 16       // Size of the salt
#define KEY_SIZE 32        // AES-256 requires 32-byte key
#define IV_SIZE 16         // AES block size is 16 bytes
#define ITERATIONS 600000  // Number of iterations for PBKDF2 hashing, OWASP recommends at least 600,000
#define BUFFER_SIZE 4096   // Buffer size for reading files

// Function to derive an encryption key from a password using PBKDF2
void derive_key(const char *password, unsigned char *salt, unsigned char *key) {
    if (PKCS5_PBKDF2_HMAC(password, strlen(password), salt, SALT_SIZE, ITERATIONS, EVP_sha256(), KEY_SIZE, key) != 1) {
        fprintf(stderr, "Error deriving key from password\n");
        exit(1);
    }
}

// Function to encrypt a file using AES-256-CBC
int encrypt_file(const char *input_file, const char *output_file, const char *password) {
    FILE *in_file = fopen(input_file, "rb");
    FILE *out_file = fopen(output_file, "wb");
    if (!in_file || !out_file) {
        perror("Error opening file");
        return 1;
    }

    unsigned char salt[SALT_SIZE];
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char buffer[BUFFER_SIZE];
    unsigned char encrypted_buffer[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];

    // Generate a random salt and write it to the output file
    if (RAND_bytes(salt, SALT_SIZE) != 1) {
        fprintf(stderr, "Error generating random salt\n");
        return 1;
    }
    fwrite(salt, 1, SALT_SIZE, out_file);

    // Derive the encryption key from the password and salt
    derive_key(password, salt, key);

    // Generate a random IV and write it to the output file
    if (RAND_bytes(iv, IV_SIZE) != 1) {
        fprintf(stderr, "Error generating IV\n");
        return 1;
    }
    fwrite(iv, 1, IV_SIZE, out_file);

    // Initialize the encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Error initializing cipher context\n");
        return 1;
    }
    EVP_EncryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL);

    // Encrypt the file contents
    int len, encrypted_len;
    while ((len = fread(buffer, 1, BUFFER_SIZE, in_file)) > 0) {
        EVP_EncryptUpdate(ctx, encrypted_buffer, &encrypted_len, buffer, len);
        fwrite(encrypted_buffer, 1, encrypted_len, out_file);
    }
    EVP_EncryptFinal_ex(ctx, encrypted_buffer, &encrypted_len);
    fwrite(encrypted_buffer, 1, encrypted_len, out_file);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(in_file);
    fclose(out_file);
    return 0;
}

// Function to decrypt a file using AES-256-CBC
int decrypt_file(const char *input_file, const char *output_file, const char *password) {
    FILE *in_file = fopen(input_file, "rb");
    FILE *out_file = fopen(output_file, "wb");
    if (!in_file || !out_file) {
        perror("Error opening file");
        return 1;
    }

    unsigned char salt[SALT_SIZE];
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char buffer[BUFFER_SIZE];
    unsigned char decrypted_buffer[BUFFER_SIZE];

    // Read the salt from the input file
    fread(salt, 1, SALT_SIZE, in_file);

    // Derive the decryption key from the password and salt
    derive_key(password, salt, key);

    // Read the IV from the input file
    fread(iv, 1, IV_SIZE, in_file);

    // Initialize the decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Error initializing cipher context\n");
        return 1;
    }
    EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL);

    // Decrypt the file contents
    int len, decrypted_len;
    while ((len = fread(buffer, 1, BUFFER_SIZE, in_file)) > 0) {
        EVP_DecryptUpdate(ctx, decrypted_buffer, &decrypted_len, buffer, len);
        fwrite(decrypted_buffer, 1, decrypted_len, out_file);
    }
    if (EVP_DecryptFinal_ex(ctx, decrypted_buffer, &decrypted_len) <= 0) {
        fprintf(stderr, "Decryption failed. Wrong password or corrupted file.\n");
        return 1;
    }
    fwrite(decrypted_buffer, 1, decrypted_len, out_file);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(in_file);
    fclose(out_file);
    return 0;
}

int main() {
    char password[256];
    char input_file[256];
    char output_file[256];
    int choice;

    printf("1. Encrypt a file\n2. Decrypt a file\nChoose an option: ");
    scanf("%d", &choice);
    getchar();

    // Prompt the user for the necessary information
    if (choice == 1) {
        printf("Enter the file to encrypt: ");
        fgets(input_file, sizeof(input_file), stdin);
        input_file[strcspn(input_file, "\n")] = 0;

        printf("Enter the output encrypted file name: ");
        fgets(output_file, sizeof(output_file), stdin);
        output_file[strcspn(output_file, "\n")] = 0;

        printf("Enter the password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        // Encrypt the file
        if (encrypt_file(input_file, output_file, password) == 0) {
            printf("File encrypted successfully!\n");
        }

    } else if (choice == 2) {
        printf("Enter the file to decrypt: ");
        fgets(input_file, sizeof(input_file), stdin);
        input_file[strcspn(input_file, "\n")] = 0;

        printf("Enter the output decrypted file name: ");
        fgets(output_file, sizeof(output_file), stdin);
        output_file[strcspn(output_file, "\n")] = 0;

        printf("Enter the password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        // Decrypt the file
        if (decrypt_file(input_file, output_file, password) == 0) {
            printf("File decrypted successfully!\n");
        }
    } else {
        printf("Invalid option\n");
    }

    return 0;
}
