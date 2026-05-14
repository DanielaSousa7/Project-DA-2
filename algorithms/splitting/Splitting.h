/**
* @file Splitting.h
 * @brief Interface for register allocation with web splitting.
 *
 * Provides functions to split webs at chosen program points to reduce
 * interference, enabling coloring with a limited number of registers.
 */

#ifndef SPLITTING_H
#define SPLITTING_H

#include <vector>
#include "../../structs.h"
using namespace std;

/**
 * @brief Computes the Cartesian product of a set of integer sets.
 *
 * @param sets A vector of vectors, each containing possible split points
 *             for a particular web.
 * @return All combinations of split points, one from each set.
 *
 * @complexity O(P * S), where P is the total number of combinations and S is the number of sets.
 */
vector<vector<int>> cartesianProduct(vector<vector<int>> sets);

/**
 * @brief Attempts register allocation by splitting up to a given number of webs.
 *
 * First tries basic allocation. If that fails, tries splitting 1, 2, ... up to
 * maxSplits webs. For each combination of webs to split, tries all possible
 * split points (between start and end) and tests if the resulting graph is
 * K-colorable (K = maxRegisters). Returns the first successful allocation.
 *
 * @param webs List of webs (variable live ranges).
 * @param maxRegisters Number of available physical registers.
 * @param maxSplits Maximum number of webs allowed to be split.
 * @return AllocationResult indicating success/failure and register assignment.
 *
 * @complexity O( sum_{s=1..maxSplits} C(N,s) * R^s * (N+s)^2 ), where N is the number of webs and R is the average number of possible split points per web.
 */
AllocationResult splittingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSplits);

#endif