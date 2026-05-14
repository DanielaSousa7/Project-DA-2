/**
 * @file Allocation.h
 * @brief Provides the interface for running register allocation algorithms.
 *
 * This module defines the main entry points for performing register allocation
 * based on live ranges input and algorithm settings. It builds webs from live
 * ranges and invokes the appropriate allocation strategy (basic, spilling,
 * splitting, or free).
 */

#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <string>
#include <vector>
#include "structs.h"
using namespace std;

/**
 * @brief Runs the complete register allocation pipeline.
 *
 * Builds webs from the live ranges, then calls allocateWithAlgorithm() using
 * the settings provided in the input data.
 *
 * @param data Parsed input containing live ranges, register settings,
 *             and algorithm configuration.
 * @return AllocationResult structure containing the assignment of registers
 *         (or memory) to each web and a success flag.
 *
 * @complexity O(V + E) for building webs + O(allocateWithAlgorithm).
 *             See allocateWithAlgorithm for detailed complexity.
 */
AllocationResult runRegisterAllocation(InputData data);

/**
 * @brief Selects and executes the allocation algorithm according to settings.
 *
 * Dispatches to one of the concrete allocation implementations:
 * - "basic"      -> basicRegisterAllocation()
 * - "spilling"   -> spillingRegisterAllocation()
 * - "splitting"  -> splittingRegisterAllocation()
 * - "free"       -> freeRegisterAllocation()
 * Defaults to basic if algorithm name is unrecognised.
 *
 * @param webs Vector of webs built from live ranges.
 * @param settings Register settings (number of registers, algorithm name,
 *                 optional parameter for spilling/splitting).
 * @return AllocationResult with assignment and success status.
 *
 * @complexity Depends on the dispatched algorithm:
 *             - Basic: O(N^2) for greedy coloring on N nodes.
 *             - Spilling: O(K * N^2) where K is the spilling limit.
 *             - Splitting: O(N^3) in worst case due to web splitting and re-coloring.
 *             - Free: User-defined; documented separately.
 */
AllocationResult allocateWithAlgorithm(vector<Web> webs, RegisterSettings settings);

#endif