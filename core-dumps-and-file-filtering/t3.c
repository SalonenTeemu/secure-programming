/*
 * This program enables or disables core dump files dynamically based on user input using setrlimit function with RLIMIT_CORE resource limit.
 * The program also simulates a crash (segmentation fault) to demonstrate core dump behavior.
 *
 * A core dump is a file that captures the memory contents of a process at the moment it crashes.
 * Core dumps can be useful for debugging, as they allow inspecting the state of a program when it crashed.
 * However, there are several issues with core dumps or similar files:
 *  - They can contain sensitive information (e.g., passwords, encryption keys).
 *  - Dumps can be exploited to steal or leak sensitive information.
 *  - They can be large and consume disk space.
 *  - They can be used to analyze and exploit vulnerabilities.
 *
 * - https://man7.org/linux/man-pages/man2/getrlimit.2.html
 *
 * First, set the ulimit to unlimited using the command: ulimit -c unlimited
 * Then set the core dump path to the current directory using the command: sudo sysctl -w kernel.core_pattern="core.%e.%p"
 * Compile with: gcc -g -o t3 t3.c
 * View the core dump file using the following command: gdb ./t3 core.t3.<PID>
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

// Function to enable core dumps by setting the core dump size limit to unlimited
void enable_core_dumps() {
    struct rlimit rl;

    // Set core dump size to unlimited
    rl.rlim_cur = RLIM_INFINITY;  // Current/soft limit
    rl.rlim_max = RLIM_INFINITY;  // Maximum limit

    // Apply the limit using setrlimit
    if (setrlimit(RLIMIT_CORE, &rl) == -1) {
        perror("Error enabling core dumps");
        exit(1);
    }

    printf("Core dumps are enabled.\n");
}

// Function to disable core dumps by setting the core dump size limit to zero
void disable_core_dumps() {
    struct rlimit rl;

    // Set core dump size to zero
    rl.rlim_cur = 0;  // Current/soft limit
    rl.rlim_max = 0;  // Maximum limit

    // Apply the limit using setrlimit
    if (setrlimit(RLIMIT_CORE, &rl) == -1) {
        perror("Error disabling core dumps");
        exit(1);
    }
    printf("Core dumps are disabled.\n");
}

int main() {
    char choice;

    // Prompt the user to choose whether to enable or disable core dumps
    printf("Do you want to enable core dumps? (y/n): ");
    scanf(" %c", &choice);

    // Enable or disable core dumps based on user input
    if (choice == 'y' || choice == 'Y') {
        enable_core_dumps();
    } else {
        disable_core_dumps();
    }

    // Simulate a crash by causing a segmentation fault
    int *ptr = NULL;  // Null pointer
    *ptr = 42;        // Dereferencing the null pointer causes a segmentation fault

    return 0;  // This will never be reached due to the crash
}
