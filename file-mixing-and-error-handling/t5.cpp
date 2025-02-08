/*
 * This is a basic calculator program that performs basic arithmetic operations (+, -, *, /).
 * The program tries to take care of possible underflow, overflow, and division by zero errors.
 * The program throws exceptions with appropriate messages for each case.
 * Example input: 1e+100 * 2 = 2e+100
 *
 * References:
 *  - std::numeric_limits: https://en.cppreference.com/w/cpp/types/numeric_limits
 *  - std::invalid_argument: https://en.cppreference.com/w/cpp/error/invalid_argument
 *  - std::overflow_error: https://en.cppreference.com/w/cpp/error/overflow_error
 *  - std::isfinite: https://en.cppreference.com/w/cpp/numeric/math/isfinite
 *  - https://labex.io/tutorials/cpp-how-to-handle-numeric-limits-in-c-418998
 */

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

// Constants for input range limits
const long double INPUT_MIN = -numeric_limits<long double>::max();
const long double INPUT_MAX = numeric_limits<long double>::max();
const long double INPUT_MIN_POSITIVE = numeric_limits<long double>::min();

// Function to check if the number is smaller than the minimum allowed positive value
void checkSmallValue(long double num) {
    if (num < INPUT_MIN_POSITIVE && num > 0) {
        throw invalid_argument("Error: Input is smaller than the minimum positive value.");
    }
}

// Function to add two numbers and check for overflow
long double safeAddition(long double num1, long double num2) {
    if (num2 > 0 && num1 > INPUT_MAX - num2 || num2 < 0 && num1 < INPUT_MIN - num2) {
        throw overflow_error("Error: Overflow occurred during addition.");
    }
    return num1 + num2;
}

// Function to subtract two numbers and check for overflow
long double safeSubtraction(long double num1, long double num2) {
    if (num2 > 0 && num1 < INPUT_MIN + num2 || num2 < 0 && num1 > INPUT_MAX + num2) {
        throw overflow_error("Error: Overflow occurred during subtraction.");
    }
    return num1 - num2;
}

// Function to multiply two numbers and check for overflow
long double safeMultiplication(long double num1, long double num2) {
    if (num1 > 0 && num2 > 0) {
        if (num1 > INPUT_MAX / num2) {
            throw overflow_error("Error: Overflow occurred during multiplication.");
        }
    } else if (num1 > 0 && num2 < 0) {
        if (num2 < INPUT_MIN / num1) {
            throw overflow_error("Error: Overflow occurred during multiplication.");
        }
    } else if (num1 < 0 && num2 > 0) {
        if (num1 < INPUT_MIN / num2) {
            throw overflow_error("Error: Overflow occurred during multiplication.");
        }
    }
    return num1 * num2;
}

// Function to divide two numbers and check for division by zero
long double safeDivision(long double num1, long double num2) {
    if (num2 == 0) {
        throw invalid_argument("Error: Division by zero is not allowed.");
    }

    if (!isfinite(num1) || !isfinite(num2)) {
        throw invalid_argument("Error: Invalid input for division.");
    }
    return num1 / num2;
}

// Function to validate input within the allowed range
long double getNumberInput() {
    long double num;
    while (true) {
        cin >> num;
        // Check if input is a valid number
        if (cin.fail()) {
            cin.clear();                                          // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            cout << "Invalid input. Please enter a valid number: ";
        } else if (num < INPUT_MIN || num > INPUT_MAX) {
            cout << "Error: Input exceeds the allowed range (" << INPUT_MIN
                 << " to " << INPUT_MAX << "). Try again: ";
        } else {
            // Check if the number is smaller than the minimum allowed positive value
            try {
                checkSmallValue(num);
            } catch (const exception& e) {
                cout << e.what() << endl;
                cout << "Please enter a valid number within the allowable range: ";
                continue;  // Skip to the next iteration if the number is invalid
            }
            break;
        }
    }
    return num;  // Return valid input
}

// Function to validate operation input
char getOperationInput() {
    char operation;
    while (true) {
        cin >> operation;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid operation (+, -, *, /): ";
        } else if (operation != '+' && operation != '-' && operation != '*' && operation != '/') {
            cout << "Invalid input. Please enter a valid operation (+, -, *, /): ";
        } else {
            break;
        }
    }
    return operation;  // Return valid operation
}

int main() {
    long double num1, num2;
    char operation;

    cout << "Enter the first number: ";
    num1 = getNumberInput();

    cout << "Enter the operation (+, -, *, /): ";
    operation = getOperationInput();

    cout << "Enter the second number: ";
    num2 = getNumberInput();

    try {
        long double result;
        switch (operation) {
            case '+': {
                result = safeAddition(num1, num2);
                cout << "Result: " << result << endl;
                break;
            }
            case '-': {
                result = safeSubtraction(num1, num2);
                cout << "Result: " << result << endl;
                break;
            }
            case '*': {
                result = safeMultiplication(num1, num2);
                cout << "Result: " << result << endl;
                break;
            }
            case '/': {
                result = safeDivision(num1, num2);
                cout << "Result: " << result << endl;
                break;
            }
            default:
                cout << "Invalid input. Please enter a valid operation (+, -, *, /): " << endl;
                break;
        }
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return EXIT_SUCCESS;
}
