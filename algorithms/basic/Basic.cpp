/**
 * @file Basic.cpp
 * @brief Implementation of basic greedy register allocation (Chaitin-style).
 */

#include "Basic.h"
#include "../../WebBuilder.h"

#include <vector>

using namespace std;

/**
 * @brief Attempt K-coloring using simplification and greedy assignment.
 *
 * @param graph Interference graph.
 * @param K Number of colors.
 * @param colorMap Output coloring.
 * @return True if successful.
 *
 * @complexity O(N^2) as described in the header.
 */
bool tryColorWithK(InterferenceGraph graph, int K, vector<int>& colorMap) {
    int n = graph.webs.size();

    vector<int> degree(n, 0);
    vector<bool> removed(n, false);
    vector<vector<bool>> remainingAdj = graph.adj;
    vector<int> stack;

    // Initialize degrees
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (remainingAdj[i][j]) degree[i]++;
        }
    }

    // Simplification phase
    while (true) {
        int node = -1;
        // Find a node with degree < K
        for (int i = 0; i < n; ++i) {
            if (!removed[i] && degree[i] < K) {
                node = i;
                break;
            }
        }
        if (node == -1) {
            // No low-degree node found
            int remaining = 0;
            for (int i = 0; i < n; ++i) {
                if (!removed[i]) remaining++;
            }
            if (remaining == 0) break;   // Graph empty -> success
            return false;                // All remaining nodes have degree >= K -> not K-colorable
        }
        removed[node] = true;
        stack.push_back(node);
        // Decrease degree of neighbors
        for (int u = 0; u < n; ++u) {
            if (!removed[u] && remainingAdj[node][u]) {
                degree[u]--;
            }
        }
    }

    // Assignment phase (reverse order)
    vector<int> colors(n, -1);
    for (int i = stack.size() - 1; i >= 0; --i) {
        int node = stack[i];
        vector<bool> usedColors(K, false);
        // Mark colors already used by colored neighbors
        for (int u = 0; u < n; ++u) {
            if (u != node && colors[u] != -1 && graph.adj[node][u]) {
                usedColors[colors[u]] = true;
            }
        }
        int chosen = -1;
        for (int c = 0; c < K; ++c) {
            if (!usedColors[c]) {
                chosen = c;
                break;
            }
        }
        if (chosen == -1) return false; // Should not happen if simplification succeeded
        colors[node] = chosen;
    }
    colorMap = colors;
    return true;
}

/**
 * @brief Finds the minimal number of registers (up to maxRegisters) that works.
 *
 * @param webs List of webs.
 * @param maxRegisters Upper bound on registers to try.
 * @return AllocationResult.
 *
 * @complexity O(maxRegisters * N^2).
 */
AllocationResult basicRegisterAllocation(vector<Web> webs, int maxRegisters) {
    AllocationResult result;
    result.success = false;
    result.usedRegisters = 0;
    result.webToRegister.assign(webs.size(), -1);
    if (webs.empty()) {
        result.success = true;
        return result;
    }

    InterferenceGraph graph = buildInterferenceGraph(webs);

    for (int K = 1; K <= maxRegisters; ++K) {
        vector<int> colors;
        if (tryColorWithK(graph, K, colors)) {
            result.success = true;
            result.usedRegisters = K;
            result.webToRegister = colors;
            return result;
        }
    }
    return result;
}