/**
* @file Spilling.h
 * @brief Interface for register allocation with web spilling.
 *
 * Provides function to spill (remove) webs from the interference graph
 * when coloring is not possible with the given number of registers.
 */

#ifndef SPILLING_H
#define SPILLING_H

#include <vector>
#include "../../structs.h"
using namespace std;

/**
 * @brief Attempts allocation by spilling up to a given number of webs.
 *
 * Tries basic allocation first. If it fails, iteratively spills 1, 2, ... up to
 * maxSpill webs using brute-force selection and attempts basic allocation on
 * the reduced graph. Returns the first successful allocation.
 *
 * @param webs List of webs.
 * @param maxRegisters Number of available registers.
 * @param maxSpill Maximum number of webs allowed to be spilled.
 * @return AllocationResult with success flag, used registers, and register mapping.
 *
 * @complexity O( sum_{s=1..maxSpill} C(N,s) * N² ), where N is the number of webs.
 */
AllocationResult spillingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSpill);

#endif