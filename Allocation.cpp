/**
 * @file Allocation.cpp
 * @brief Implementation of the register allocation dispatcher.
 *
 * Implements the functions declared in Allocation.h. Builds webs from live
 * ranges and chooses the allocation algorithm based on the input settings.
 */

#include "Allocation.h"
#include "WebBuilder.h"

#include "algorithms/basic/Basic.h"
#include "algorithms/spilling/Spilling.h"
#include "algorithms/splitting/Splitting.h"
#include "algorithms/free/Free.h"

#include <fstream>
#include <iostream>

using namespace std;

/**
 * @brief Constructs webs from live ranges and runs the chosen allocation algorithm.
 *
 * @param data Input data containing live ranges and allocation settings.
 * @return AllocationResult with the final assignment.
 *
 * @complexity Let R = number of live ranges, V = number of variables.
 *             Building webs: O(R) using union-find or interval merging.
 *             Allocation: see allocateWithAlgorithm().
 *             Overall: O(R + T) where T is complexity of the selected algorithm.
 */
AllocationResult runRegisterAllocation(InputData data) {
    vector<Web> webs = buildWebs(data.liveRanges);
    AllocationResult result = allocateWithAlgorithm(webs, data.settings);
    result.webs = webs;
    return result;
}

/**
 * @brief Dispatches to the requested allocation algorithm.
 *
 * @param webs Webs representing variables/intervals.
 * @param settings Algorithm and register configuration.
 * @return AllocationResult after running the algorithm.
 *
 * @complexity See allocateWithAlgorithm() documentation in header.
 */
AllocationResult allocateWithAlgorithm(vector<Web> webs, RegisterSettings settings) {
    if (settings.algorithm == "basic") {
        return basicRegisterAllocation(webs, settings.registers);
    }

    if (settings.algorithm == "spilling") {
        return spillingRegisterAllocation(webs, settings.registers, settings.param);
    }

    if (settings.algorithm == "splitting") {
        return splittingRegisterAllocation(webs, settings.registers, settings.param);
    }

    if (settings.algorithm == "free") {
        return freeRegisterAllocation(webs, settings.registers);
    }

    return basicRegisterAllocation(webs, settings.registers);
}