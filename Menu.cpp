/**
 * @file Menu.cpp
 * @brief Implementation of the interactive menu.
 */

#include "Menu.h"
#include "Parser.h"
#include "structs.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

void showMainMenu() {
    cout << "=== Assignment Tool ===\n";
    cout << "1. Read new file\n";
    cout << "2. Create Output file\n";
    cout << "0. Exit\n";
    cout << "Select an option: ";
}

int mainMenu() {
    InputData data;
    bool filesLoaded = false;

    // Initial loading: ask for both input files until successful
    while (!filesLoaded) {
        string rangesFile, registersFile;
        cout << "Enter live ranges file path: ";
        cin >> rangesFile;
        cout << "Enter registers file path: ";
        cin >> registersFile;
        cout << endl;

        // Parse with an empty output filename (will default to "output.txt")
        data = parseInputFiles(rangesFile, registersFile, "");
        if (!data.hasErrors) {
            filesLoaded = true;
            cout << "Files loaded successfully.\n\n";
        } else {
            cout << "Invalid file(s). Please try again.\n\n";
        }
    }

    int option = -1;
    while (option != 0) {
        showMainMenu();
        if (cin >> option) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch(option) {
                case 1: {
                    // Reload files
                    string rangesFile, registersFile;
                    cout << "Enter live ranges file path: ";
                    cin >> rangesFile;
                    cout << "Enter registers file path: ";
                    cin >> registersFile;
                    // Keep the currently set output filename
                    data = parseInputFiles(rangesFile, registersFile, data.outputFile);
                    if (!data.hasErrors) {
                        cout << "Files loaded successfully.\n";
                    } else {
                        cout << "Errors occurred. Data not updated.\n";
                    }
                    break;
                }
                case 2: {
                    string output_filename;
                    cout << "Enter the name of Output file: ";
                    cin >> output_filename;
                    data.outputFile = output_filename;
                    cout << "Output file set to: " << data.outputFile << "\n";
                    break;
                }
                case 0:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid option!\n";
                    break;
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option!\n";
            option = -1;
        }
        cout << "\n";
    }
    return 0;
}