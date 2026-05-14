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
 * @complexity O(P * S) where P is the total number of combinations
 *             (product of sizes of input sets) and S is the number of sets.
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
 * @complexity Let N = number of webs, S = maxSplits, K = maxRegisters.
 *             In worst case, O( N^S * (range)^S * C(N*2) ) where C(m) is the
 *             complexity of basic allocation on m nodes (O(m^2) greedy).
 *             Very high combinatorial cost; practical only for small S.
 */
AllocationResult splittingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSplits);

#endif