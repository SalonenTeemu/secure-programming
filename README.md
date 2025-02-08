# secure-programming

This repository contains programs created in the Secure Programming course as independent course exercises. Each simple program demonstrates specific security concepts and/or vulnerabilities, organized into distinct sections.

## Sections

### 1. Overflow

This section focuses on various types of buffer overflows and related vulnerabilities.

- **t1.c**: A program that causes unlimited buffer overflow.
- **t2.c**: A program that causes off-by-one overflow.
- **t3.c**: A program that demonstrates heap overflow.
- **t4.c**: A program that exploits function pointer overflow.
- **t5.c**: A program showcasing a `printf` vulnerability.

---

### 2. Encryption and Authentication

This section covers secure cryptographic operations and authentication mechanisms.

- **t2.c**: A program that writes cryptographically secure random data to a file.
- **t3.c**: A program for user authentication using a username and password. Password hashes are stored in a file.
- **t4.c**: A program to encrypt and decrypt files.

---

### 3. Core Dump Files and File Filtering

This section handles core dump files and disabling them as well as filtering out unwanted characters from files.

- **t3.c**: A program with that prevents core dump files apppearing.
- **t4.c**: A program that reads a file and filters all characters except letter’s, numbers, commas and hyphen and prints the result on the screen.

---

### 4. File Mixing and Error Handling

This section handles errors related to files as well as underflowing or overflowing numbers.

- **t4.cpp**: A program that mixes two files row by row: first row from the other, second from the other and so on while checking for possible errors and throwing exceptions with appropriate messages for each case.
- **t5.cpp**: A command line interface calculator program that checks for possible underflow, overflow, and division by zero errors and throws exceptions with appropriate messages for each case.

---
