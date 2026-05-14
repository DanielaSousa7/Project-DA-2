#ifndef PROJECT_OUTPUT_H
#define PROJECT_OUTPUT_H

#include "Structs.h"
#include <string>
using namespace std;

/**
 * @brief Writes the register allocation result to an output file.
 *
 * @param result Allocation result produced by the algorithm.
 * @param webs Vector containing the program webs.
 * @param outputFile Name of the output file.
 *
 * @complexity O(w * p), where w is the number of webs and p is the number of program points per web.
 */
void writeAllocationOutput(AllocationResult result, vector<Web> webs, string outputFile);

#endif