/*
 * This program reads two given files line by line and writes them to a given third file.
 * The lines are written in a mixed order: first line from the other, second from the other and so on.
 * The program handles different errors that may occur during file operations, including:
 *  - Opening input files for reading
 *  - Creating the output file for writing
 *  - Writing to the output file
 *  - Closing the files
 * The program checks for read, write, and close errors separately, and throws exceptions with appropriate messages for each case.
 *
 * References:
 * - std::runtime_error: https://en.cppreference.com/w/cpp/error/runtime_error
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// Helper function to handle errors by throwing a runtime exception with the message
void handle_error(const string& message) {
    throw runtime_error(message);
}

int main() {
    string file1_name, file2_name, output_file_name;
    ifstream file1, file2;
    ofstream output_file;
    string line1, line2;

    // Ask user for file names
    cout << "Enter the name of the 1st (input) file: ";
    cin >> file1_name;

    cout << "Enter the name of the 2nd (input) file: ";
    cin >> file2_name;

    cout << "Enter the name of the 3rd (output) file: ";
    cin >> output_file_name;

    try {
        // Open first file
        file1.open(file1_name);
        if (!file1) {
            handle_error("Error: Could not open file '" + file1_name + "'.");
        }

        // Open second file
        file2.open(file2_name);
        if (!file2) {
            file1.close();
            handle_error("Error: Could not open file '" + file2_name + "'.");
        }

        // Open output file
        output_file.open(output_file_name);
        if (!output_file) {
            file1.close();
            file2.close();
            handle_error("Error: Could not create output file '" + output_file_name + "'.");
        }

        // Mix the files row by row
        while (true) {
            bool file1_read = static_cast<bool>(getline(file1, line1));
            bool file2_read = static_cast<bool>(getline(file2, line2));

            // Check for errors other than EOF
            if (file1.fail() && !file1.eof()) {
                handle_error("Error: Failed to read from file '" + file1_name + "'.");
            }

            if (file2.fail() && !file2.eof()) {
                handle_error("Error: Failed to read from file '" + file2_name + "'.");
            }

            // Check if both files are done
            if (!file1_read && !file2_read) {
                break;
            }

            if (file1_read) {
                // Write line to output file
                output_file << line1 << endl;
                if (output_file.fail()) {
                    handle_error("Error: Failed to write to output file '" + output_file_name + "'.");
                }
            }

            if (file2_read) {
                // Write line to output file
                output_file << line2 << endl;
                if (output_file.fail()) {
                    handle_error("Error: Failed to write to output file " + output_file_name + "'.");
                }
            }
        }

        // Close all files
        file1.close();
        file2.close();
        output_file.close();

        // Check for errors after closing files
        if (file1.fail() || file2.fail() || output_file.fail()) {
            handle_error("Error: Failed to close one or more files properly.");
        }

        cout << "Files successfully mixed into '" << output_file_name << "'." << endl;
    } catch (const runtime_error& e) {
        cerr << "Runtime Error: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cerr << "General Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
