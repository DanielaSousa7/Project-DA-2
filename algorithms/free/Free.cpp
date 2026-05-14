/**
 * @file Free.cpp
 * @brief Implementation of free (custom) register allocation.
 *
 * Uses DSATUR coloring and iterative highest-degree web spilling.
 */

#include "Free.h"
#include "../../WebBuilder.h"

#include <numeric>
#include <set>
#include <vector>

using namespace std;

/**
 * @brief DSATUR graph coloring algorithm.
 *
 * @param graph Interference graph.
 * @param K Number of colors.
 * @param colorMap Output color assignment.
 * @return true if successful.
 *
 * @complexity O(N^2) as described.
 */
bool tryDSATUR(InterferenceGraph graph, int K, vector<int>& colorMap) {
    int n = graph.webs.size();
    vector<int> colors(n, -1);
    vector<int> degree(n, 0);
    vector<set<int>> adjColors(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph.adj[i][j]) {
                degree[i]++;
            }
        }
    }
    for (int assigned = 0; assigned < n; ++assigned) {
        int bestNode = -1;
        int maxSat = -1;
        int maxDeg = -1;
        for (int i = 0; i < n; ++i) {
            if (colors[i] != -1) continue;
            int sat = (int)adjColors[i].size();
            if (sat > maxSat || (sat == maxSat && degree[i] > maxDeg)) {
                maxSat = sat;
                maxDeg = degree[i];
                bestNode = i;
            }
        }
        if (bestNode == -1) break;
        int chosenColor = -1;
        for (int c = 0; c < K; ++c) {
            if (adjColors[bestNode].find(c) == adjColors[bestNode].end()) {
                chosenColor = c;
                break;
            }
        }
        if (chosenColor == -1) return false;
        colors[bestNode] = chosenColor;
        for (int u = 0; u < n; ++u) {
            if (colors[u] == -1 && graph.adj[bestNode][u]) {
                adjColors[u].insert(chosenColor);
            }
        }
    }
    colorMap = colors;
    return true;
}

/**
 * @brief Custom allocation: DSATUR + iterative spilling of highest-degree web.
 *
 * @param webs Original webs.
 * @param maxRegisters Number of registers.
 * @return AllocationResult.
 *
 * @complexity O(N^3) worst case.
 */
AllocationResult freeRegisterAllocation(vector<Web> webs, int maxRegisters) {
    int n = webs.size();
    vector<Web> currentWebs = webs;
    vector<int> originalIdx(n);
    iota(originalIdx.begin(), originalIdx.end(), 0);

    while (true) {
        if (!currentWebs.empty()) {
            InterferenceGraph graph = buildInterferenceGraph(currentWebs);
            vector<int> colors;
            if (tryDSATUR(graph, maxRegisters, colors)) {
                AllocationResult result;
                result.algorithm_used = "free";
                result.success = true;
                result.usedRegisters = maxRegisters;
                result.webToRegister.assign(n, -1);
                for (size_t i = 0; i < currentWebs.size(); ++i) {
                    int original = originalIdx[i];
                    result.webToRegister[original] = colors[i];
                }
                result.webs = webs;
                return result;
            }
        }
        InterferenceGraph currentGraph = buildInterferenceGraph(currentWebs);
        int spillIdx = -1;
        int maxDegree = -1;
        for (size_t i = 0; i < currentWebs.size(); ++i) {
            int degree = 0;
            for (size_t j = 0; j < currentWebs.size(); ++j) {
                if (currentGraph.adj[i][j]) {
                    degree++;
                }
            }
            if (degree > maxDegree) {
                maxDegree = degree;
                spillIdx = i;
            }
        }
        if (spillIdx == -1) break;
        currentWebs.erase(currentWebs.begin() + spillIdx);
        originalIdx.erase(originalIdx.begin() + spillIdx);
        if (currentWebs.empty()) break;
    }
    AllocationResult fail;
    fail.success = false;
    fail.usedRegisters = 0;
    fail.webToRegister.assign(n, -1);
    return fail;
}