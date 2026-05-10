/**
* @file menu.h
 * @brief Interactive menu functions for the conference assignment tool.
 */

#ifndef PROJECT_DA_2_MENU_H
#define PROJECT_DA_2_MENU_H

#include "Structs.h"

void showMainMenu();
void showDataMenu();
void printProgramPoint(ProgramPoint point);
void printLiveRanges(InputData data);
void printRegisterSettings(InputData data);
void dataMenu(InputData data);
int mainMenu();

#endif