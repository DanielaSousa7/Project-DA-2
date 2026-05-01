/**
* @file menu.h
 * @brief Interactive menu functions for the conference assignment tool.
 */

#ifndef PROJECT_DA_2_MENU_H
#define PROJECT_DA_2_MENU_H

#include "structs.h"

void showMainMenu();
void printProgramPoint(ProgramPoint point);
void printLiveRanges(InputData data);
void printRegisterSettings(InputData data);
void printLoadedData(InputData data);
int mainMenu();

#endif