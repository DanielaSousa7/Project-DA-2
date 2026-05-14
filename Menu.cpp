/**
 * @file Menu.cpp
 * @brief Implementation of the interactive menu for the register allocation tool.
 *
 * Implements all menu display and interaction logic, including file loading,
 * data browsing, output file configuration, and invoking the allocation pipeline.
 */

#include "Menu.h"
#include "Parser.h"
#include "Allocation.h"
#include "WebBuilder.h"
#include "Output.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

/**
 * @brief Prints the main menu options.
 *
 * @complexity O(1).
 */
void showMainMenu() {
    cout << "=== Assignment Tool ===\n";
    cout << "1. Read new files\n";
    cout << "2. Browse File Data\n";
    cout << "3. Create Output file\n";
    cout << "4. Run Register Allocation\n";
    cout << "0. Exit\n";
    cout << "Select an option: ";
}

/**
 * @brief Prints the data browsing sub‑menu.
 *
 * @complexity O(1).
 */
void showDataMenu() {
    cout << "=== File Data ===\n";
    cout << "1. Show Live Ranges\n";
    cout << "2. Show Register Settings\n";
    cout << "0. Back\n";
    cout << "Select an option: ";
}

/**
 * @brief Prints a single program point with definition/use markers.
 *
 * @param point The program point.
 *
 * @complexity O(1).
 */
void printProgramPoint(ProgramPoint point) {
    cout << point.code_line;
    if (point.isDefinition) {
        cout << "+";
    }
    if (point.isLastUse) {
        cout << "-";
    }
}

/**
 * @brief Prints all live ranges.
 *
 * @param data Input data.
 *
 * @complexity O(L * P) linear in total points.
 */
void printLiveRanges(InputData data) {
    cout << "--- Live Ranges ---\n";
    for (LiveRange range : data.liveRanges) {
        cout << range.variable << ": ";
        for (size_t i = 0; i < range.points.size(); i++) {
            printProgramPoint(range.points[i]);
            if (i + 1 < range.points.size()) {
                cout << ",";
            }
        }
        cout << "\n";
    }
}

/**
 * @brief Prints register settings.
 *
 * @param data Input data.
 *
 * @complexity O(1).
 */
void printRegisterSettings(InputData data) {
    cout << "--- Register Settings ---\n";
    cout << "Registers: " << data.settings.registers << "\n";
    cout << "Algorithm: " << data.settings.algorithm;
    if (data.settings.param > 0)
        cout << ", " << data.settings.param;
    cout << "\n";
    cout << "Output file: " << data.outputFile << "\n";
}

/**
 * @brief Interactive sub‑menu for browsing loaded data.
 *
 * @param data The loaded input data.
 *
 * @complexity User interaction loop; each print is O(L*P) or O(1).
 */
void dataMenu(InputData data) {
    int option = -1;
    while (option != 0) {
        showDataMenu();
        if (cin >> option) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (option) {
                case 1:
                    cout << "\n";
                    printLiveRanges(data);
                    cout << "\n";
                    break;
                case 2:
                    cout << "\n";
                    printRegisterSettings(data);
                    cout << "\n";
                    break;
                case 0:
                    break;
                default:
                    cout << "Invalid option!\n\n";
                    break;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option!\n\n";
            option = -1;
        }
    }
}

/**
 * @brief Main interactive menu loop.
 *
 * Prompts for input files, then repeatedly presents the main menu until exit.
 *
 * @return int Always returns 0.
 *
 * @complexity File parsing O(input size), allocation O(algorithm complexity),
 *             user interaction loop overhead is minimal.
 */
int mainMenu() {
    InputData data;
    bool filesLoaded = false;

    while (!filesLoaded) {
        string rangesFile, registersFile;
        cout << "Enter live ranges file path: ";
        cin >> rangesFile;
        cout << "Enter registers file path: ";
        cin >> registersFile;
        cout << endl;

        data = parseInputFiles(rangesFile, registersFile, "");
        if (!data.hasErrors) {
            filesLoaded = true;
            cout << "Files loaded successfully.\n\n";
        }
        else {
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
                    string rangesFile, registersFile;
                    cout << "Enter live ranges file path: ";
                    cin >> rangesFile;
                    cout << "Enter registers file path: ";
                    cin >> registersFile;
                    data = parseInputFiles(rangesFile, registersFile, data.outputFile);
                    if (!data.hasErrors) {
                        cout << "Files loaded successfully.\n";
                    }
                    else {
                        cout << "Errors occurred. Data not updated.\n";
                    }
                    break;
                }
                case 2:
                    cout << "\n";
                    dataMenu(data);
                    break;
                case 3: {
                    string output_filename;
                    cout << "Enter the name of Output file: ";
                    cin >> output_filename;
                    if (output_filename.size() < 4 || output_filename.substr(output_filename.size() - 4) != ".txt") {
                        output_filename += ".txt";
                    }
                    data.outputFile = output_filename;
                    cout << "Output file set to: " << data.outputFile << "\n";
                    break;
                }
                case 4: {
                    AllocationResult result = runRegisterAllocation(data);
                    writeAllocationOutput(result, result.webs, data.outputFile);
                    cout << "Register allocation completed. Output written to: " << data.outputFile << "\n";
                    break;
                }
                case 0:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid option!\n";
                    break;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option!\n";
            option = -1;
        }
        cout << "\n";
    }
    return 0;
}