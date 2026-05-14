/**
* @file main.cpp
 * @brief Entry point of the register allocation tool.
 *
 * Decides between batch mode and interactive mode based on command line arguments.
 * - If called with "-b ranges.txt registers.txt output.txt", runs batch mode.
 * - Otherwise, launches the interactive menu.
 */

#include "Menu.h"
#include "Batch.h"
#include <iostream>
#include <string>
using namespace std;

/**
 * @brief Main function – entry point.
 *
 * @param argc Number of command line arguments.
 * @param argv Argument vector.
 * @return int Exit code (0 for success).
 *
 * @complexity If batch mode, complexity depends on batchMode() which includes
 *             file parsing (O(file size)) and allocation (O(algorithm complexity)).
 *             If interactive, mainMenu() runs until user exits.
 */
int main(int argc, char* argv[]) {
    if (argc == 5 && string(argv[1]) == "-b") {
        batchMode(argv[2], argv[3], argv[4]);
        return 0;
    }
    return mainMenu();
}