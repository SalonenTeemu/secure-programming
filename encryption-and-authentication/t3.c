/**
 * This program implements user authentication with hashed passwords using PBKDF2 and a random salt.
 * It allows users to:
 *   - Register by setting a username and password (salt is generated and stored with the hash).
 *   - Authenticate by entering their username and password (password is hashed with the stored salt).
 * The password hashes are stored in a file alongside the random salt.
 *
 * - https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html
 * - https://docs.openssl.org/3.0/man3/PKCS5_PBKDF2_HMAC/
 *
 * Compile with: gcc -o t3 t3.c -lssl -lcrypto
 */

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

#define SALT_SIZE 16               // Size of the salt
#define HASH_SIZE 32               // Size of the hashed password
#define ITERATIONS 600000          // Number of iterations for PBKDF2 hashing, OWASP recommends at least 600,000
#define FILE_NAME "passwords.txt"  // File where hashed passwords and salts are stored

// Function to generate a random salt
void generate_salt(unsigned char *salt) {
    // Generate a random salt using OpenSSL's RAND_bytes function
    if (RAND_bytes(salt, SALT_SIZE) != 1) {
        fprintf(stderr, "Error generating random salt\n");
        exit(1);
    }
}

// Function to hash a password using PBKDF2 with a salt
void hash_password(const char *password, unsigned char *salt, unsigned char *hash) {
    // Hash the password using PBKDF2 with HMAC-SHA-256
    if (PKCS5_PBKDF2_HMAC(password, strlen(password), salt, SALT_SIZE, ITERATIONS, EVP_sha256(), HASH_SIZE, hash) != 1) {
        fprintf(stderr, "Error hashing password\n");
        exit(1);
    }
}

// Function to store the username, salt, and hashed password into a file
void store_password(const char *username, const unsigned char *salt, const unsigned char *hash) {
    FILE *file = fopen(FILE_NAME, "a");  // Open the password file for appending
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Write the username, salt (in hex), and hashed password (in hex) to the file
    fprintf(file, "%s:", username);
    for (int i = 0; i < SALT_SIZE; i++) {
        fprintf(file, "%02x", salt[i]);
    }
    fprintf(file, ":");
    for (int i = 0; i < HASH_SIZE; i++) {
        fprintf(file, "%02x", hash[i]);
    }
    fprintf(file, "\n");

    fclose(file);  // Close the file
}

// Function to retrieve the salt and hash for a username from the file
int get_stored_hash_and_salt(const char *username, unsigned char *salt, unsigned char *hash) {
    FILE *file = fopen(FILE_NAME, "r");  // Open the password file for reading
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    char stored_username[256];
    unsigned char stored_salt[SALT_SIZE];
    unsigned char stored_hash[HASH_SIZE];

    // Read each line in the file to find the matching username
    while (fscanf(file, "%255[^:]:", stored_username) == 1) {
        // Read the salt (in hexadecimal format)
        for (int i = 0; i < SALT_SIZE; i++) {
            if (fscanf(file, "%2hhx", &stored_salt[i]) != 1) {
                fclose(file);
                return 0;
            }
        }

        // Skip the colon separator
        fgetc(file);

        // Read the hash (in hexadecimal format)
        for (int i = 0; i < HASH_SIZE; i++) {
            if (fscanf(file, "%2hhx", &stored_hash[i]) != 1) {
                fclose(file);
                return 0;
            }
        }

        // Check if the username matches
        if (strcmp(username, stored_username) == 0) {
            memcpy(salt, stored_salt, SALT_SIZE);  // Copy the stored salt
            memcpy(hash, stored_hash, HASH_SIZE);  // Copy the stored hash
            fclose(file);
            return 1;  // Return 1 if username is found and data is successfully retrieved
        }

        fgetc(file);  // Skip the newline character
    }

    fclose(file);
    return 0;  // Return 0 if the username is not found in the file
}

// Function to authenticate the user by comparing the entered password's hash with the stored hash
int authenticate_user(const char *username, const char *password) {
    unsigned char stored_salt[SALT_SIZE];
    unsigned char stored_hash[HASH_SIZE];
    unsigned char computed_hash[HASH_SIZE];

    // Retrieve the stored salt and hash for the given username
    if (get_stored_hash_and_salt(username, stored_salt, stored_hash) == 0) {
        fprintf(stderr, "Username not found\n");
        return 0;
    }

    // Hash the entered password using the stored salt
    hash_password(password, stored_salt, computed_hash);

    // Compare the computed hash with the stored hash
    if (memcmp(computed_hash, stored_hash, HASH_SIZE) == 0) {
        printf("Authentication successful!\n");
        return 1;
    } else {
        printf("Authentication failed!\n");
        return 0;
    }
}

int main() {
    char username[256];
    char password[256];
    int choice;

    printf("1. Register\n2. Login\nChoose an option: ");
    scanf("%d", &choice);
    getchar();

    // Registration
    if (choice == 1) {
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;  // Remove the newline character

        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        // Generate a random salt
        unsigned char salt[SALT_SIZE];
        generate_salt(salt);

        // Hash the password with the generated salt
        unsigned char hash[HASH_SIZE];
        hash_password(password, salt, hash);

        // Store the username, salt, and hashed password in the file
        store_password(username, salt, hash);
        printf("User registered successfully!\n");
    }
    
    // Login
    else if (choice == 2) {
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        // Authenticate the user
        authenticate_user(username, password);
    } else {
        printf("Invalid option\n");
    }

    return 0;
}
