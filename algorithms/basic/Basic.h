/**
 * @file Basic.h
 * @brief Interface for basic greedy graph coloring register allocation.
 *
 * Implements the classic Chaitin-style simplification and greedy assignment
 * algorithm for register allocation via graph coloring.
 */

#ifndef BASIC_H
#define BASIC_H

#include <vector>
#include "../../structs.h"
using namespace std;

/**
 * @brief Attempts to color an interference graph with exactly K colors.
 *
 * The algorithm repeatedly removes nodes with degree < K (simplification),
 * pushing them onto a stack. If the graph becomes empty, it then assigns
 * colors greedily in reverse order, always using the smallest available color.
 *
 * @param graph The interference graph to color.
 * @param K Number of available colors (registers).
 * @param colorMap Output vector mapping each node (web) to a color index (0..K-1).
 * @return true if a valid K-coloring is found, false otherwise.
 *
 * @complexity O(N^2) where N = number of nodes (webs). The simplification
 *             phase scans for low-degree nodes (O(N) per removed node) and
 *             updates degrees of neighbors (O(N) per node), yielding O(N^2).
 *             Assignment phase also runs in O(N*K) ≤ O(N^2).
 */
bool tryColorWithK(InterferenceGraph graph, int K, vector<int>& colorMap);

/**
 * @brief Basic register allocation: finds the smallest K ≤ maxRegisters that works.
 *
 * Tries K = 1, 2, ..., maxRegisters using tryColorWithK(). Returns the
 * first successful allocation. If none succeeds, reports failure.
 *
 * @param webs List of webs (variable live ranges).
 * @param maxRegisters Maximum number of physical registers available.
 * @return AllocationResult containing success flag, number of registers used,
 *         and a register assignment (or -1 if allocation fails).
 *
 * @complexity O(maxRegisters * N^2). In the worst case maxRegisters ≈ N,
 *             so O(N^3). For sparse interference graphs, performance is better.
 */
AllocationResult basicRegisterAllocation(vector<Web> webs, int maxRegisters);

#endif