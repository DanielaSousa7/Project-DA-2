/**
* @file Free.h
 * @brief Interface for custom register allocation (free algorithm).
 *
 * Implements an alternative graph coloring heuristic (DSATUR) combined with
 * aggressive spilling of the highest-degree web when coloring fails.
 */

#ifndef FREE_H
#define FREE_H

#include <vector>
#include "../../structs.h"
using namespace std;

/**
 * @brief Attempts to color an interference graph with K colors using DSATUR.
 *
 * DSATUR selects the uncolored node with the most different colors among
 * neighbors (highest saturation), breaking ties by highest degree.
 *
 * @param graph Interference graph.
 * @param K Number of available colors.
 * @param colorMap Output vector mapping node index -> color (0..K-1) if successful.
 * @return true if coloring succeeded, false otherwise.
 *
 * @complexity O(N^2) where N = number of nodes (webs), due to repeated
 *             selection of highest saturation and adjacency checks.
 */
bool tryDSATUR(InterferenceGraph graph, int K, vector<int>& colorMap);

/**
 * @brief Custom register allocation using DSATUR and heuristic spilling.
 *
 * Tries to color the full graph with DSATUR. If it fails, spills the web with
 * the highest degree, removes it, and repeats. Continues until either coloring
 * succeeds or all webs are spilled.
 *
 * @param webs List of webs.
 * @param maxRegisters Number of available registers.
 * @return AllocationResult with success flag and mapping (spilled webs: -1).
 *
 * @complexity O(N^3) in worst case: each iteration removes one node and
 *             re-runs DSATUR (O(N^2)) on the reduced graph, up to N times.
 */
AllocationResult freeRegisterAllocation(vector<Web> webs, int maxRegisters);

#endif