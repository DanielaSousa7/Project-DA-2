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
 * maxSpill webs (using brute-force selection) and attempts basic allocation on
 * the reduced graph. Returns the first successful allocation.
 *
 * @param webs List of webs.
 * @param maxRegisters Number of available registers.
 * @param maxSpill Maximum number of webs allowed to be spilled (moved to memory).
 * @return AllocationResult with success flag, used registers, and register mapping
 *         (spilled webs get -1 assignment).
 *
 * @complexity Let N = number of webs. Worst-case O( N^maxSpill * C_basic(N) )
 *             where C_basic(N) = O(N^2) for basic coloring. Exponential in maxSpill.
 */
AllocationResult spillingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSpill);

#endif