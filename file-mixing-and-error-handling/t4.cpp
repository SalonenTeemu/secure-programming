/*
 * This program reads two given files line by line and writes them to a given third file.
 * The lines are written in a mixed order: first line from the other, second from the other and so on.
 * The program handles different errors that may occur during the execution, including:
 *  - Invalid file names
 *  - Empty input files
 *  - Opening input files for reading
 *  - Creating the output file for writing
 *  - Writing to the output file
 *  - Closing the files
 *
 * The program uses exceptions to handle errors and print appropriate messages to the user.
 *
 * References:
 * - std::runtime_error: https://en.cppreference.com/w/cpp/error/runtime_error
 * - std::logic_error: https://en.cppreference.com/w/cpp/error/logic_error
 * - std::invalid_argument: https://en.cppreference.com/w/cpp/error/invalid_argument
 * - std::ios_base::failure: https://en.cppreference.com/w/cpp/io/ios_base/failure
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main() {
    string file1_name, file2_name, output_file_name;
    ifstream file1, file2;
    ofstream output_file;
    string line1, line2;

    try {
        // Ask user for file names
        cout << "Enter the name of the 1st (input) file: ";
        cin >> file1_name;

        // Open first file
        file1.open(file1_name);
        if (!file1) {
            throw ios_base::failure("Could not open file '" + file1_name + "'.");
        }

        // Check if the first input file is empty
        if (file1.peek() == EOF) {
            file1.close();
            throw logic_error("The first input file is empty. Both input files must contain at least one line.");
        }

        cout << "Enter the name of the 2nd (input) file: ";
        cin >> file2_name;

        if (file1_name == file2_name) {
            throw invalid_argument("The input files must have different names.");
        }

        // Open second file
        file2.open(file2_name);
        if (!file2) {
            file1.close();
            throw ios_base::failure("Could not open file '" + file2_name + "'.");
        }

        // Check if the second input file is empty
        if (file2.peek() == EOF) {
            file1.close();
            file2.close();
            throw logic_error("The second input file is empty. Both input files must contain at least one line.");
        }

        cout << "Enter the name of the 3rd (output) file: ";
        cin >> output_file_name;

        // Check if the output file has the same name as one of the input files
        if (output_file_name == file1_name || output_file_name == file2_name) {
            throw invalid_argument("The output file must have a different name than the input files.");
        }

        // Open output file
        output_file.open(output_file_name);
        if (!output_file) {
            file1.close();
            file2.close();
            throw ios_base::failure("Could not create output file '" + output_file_name + "'.");
        }

        // Mix the files row by row
        while (true) {
            bool file1_read = static_cast<bool>(getline(file1, line1));
            bool file2_read = static_cast<bool>(getline(file2, line2));

            // Check for errors other than EOF
            if (file1.fail() && !file1.eof()) {
                throw ios_base::failure("Failed to read from file '" + file1_name + "'.");
            }

            if (file2.fail() && !file2.eof()) {
                throw ios_base::failure("Failed to read from file '" + file2_name + "'.");
            }

            // Check if both files are done
            if (!file1_read && !file2_read) {
                break;
            }

            if (file1_read) {
                // Write line to output file
                output_file << line1 << endl;
                if (output_file.fail()) {
                    throw runtime_error("Failed to write to output file '" + output_file_name + "'.");
                }
            }

            if (file2_read) {
                // Write line to output file
                output_file << line2 << endl;
                if (output_file.fail()) {
                    throw runtime_error("Failed to write to output file '" + output_file_name + "'.");
                }
            }
        }

        // Close all files
        file1.close();
        file2.close();
        output_file.close();

        // Check for errors after closing files (not EOF)
        if ((file1.fail() && !file1.eof()) || (file2.fail() && !file2.eof()) || output_file.fail()) {
            throw ios_base::failure("Failed to close one or more files properly.");
        }

        cout << "Files successfully mixed into '" << output_file_name << "'." << endl;

    } catch (const invalid_argument& e) {
        cerr << "Invalid Argument: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch (const ios_base::failure& e) {
        cerr << "I/O Error: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch (const logic_error& e) {
        cerr << "Logic Error: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch (const runtime_error& e) {
        cerr << "Runtime Error: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cerr << "General Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
