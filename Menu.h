/**
 * @file Menu.h
 * @brief Interactive menu functions for the register allocation tool.
 *
 * Provides a console-based user interface to load input files, browse data,
 * run register allocation, and generate output. The menu supports both
 * interactive mode (via mainMenu()) and batch mode (handled separately).
 */

#ifndef PROJECT_DA_2_MENU_H
#define PROJECT_DA_2_MENU_H

#include "Structs.h"

/**
 * @brief Displays the main menu options.
 *
 * Options: Read new files, Browse File Data, Create Output file,
 * Run Register Allocation, Exit.
 *
 * @complexity O(1) – constant time.
 */
void showMainMenu();

/**
 * @brief Displays the data browsing sub‑menu.
 *
 * Options: Show Live Ranges, Show Register Settings, Back.
 *
 * @complexity O(1).
 */
void showDataMenu();

/**
 * @brief Prints a single program point (line number with +/- markers).
 *
 * @param point The program point to print.
 *
 * @complexity O(1).
 */
void printProgramPoint(ProgramPoint point);

/**
 * @brief Prints all live ranges stored in the InputData structure.
 *
 * @param data The input data containing live ranges.
 *
 * @complexity O(L * P) where L is the number of live ranges and P is the
 *             average number of points per range. Linear in total input size.
 */
void printLiveRanges(InputData data);

/**
 * @brief Prints the register settings (number of registers, algorithm, parameter).
 *
 * @param data The input data containing register settings.
 *
 * @complexity O(1).
 */
void printRegisterSettings(InputData data);

/**
 * @brief Handles the data browsing sub‑menu.
 *
 * Allows the user to view live ranges and register settings interactively.
 *
 * @param data The input data to browse.
 *
 * @complexity Depends on user interaction; printing functions called are
 *             O(L*P) and O(1) respectively.
 */
void dataMenu(InputData data);

/**
 * @brief The main interactive menu loop.
 *
 * Prompts for input files, then presents a menu to load new files,
 * browse data, set output file, and run allocation. Returns when user
 * selects exit.
 *
 * @return int Exit code (0 for normal exit).
 *
 * @complexity User‑driven; file parsing O(file size), allocation O(algorithm).
 */
int mainMenu();

#endif