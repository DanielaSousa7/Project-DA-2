#include "GraphColoring.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

// ---------- buildWebs ----------
vector<Web> buildWebs(const vector<LiveRange> &liveRanges) {
    // Step 1: group all intervals per variable
    // An interval is just [min line, max line] for each LiveRange
    map< string, vector<pair<int,int>> > varIntervals;
    
    for (const auto &lr : liveRanges) {
        if (lr.points.empty()) continue;
        string var = lr.variable;
        int minLine = lr.points[0].code_line;
        int maxLine = lr.points[0].code_line;
        for (const auto &p : lr.points) {
            if (p.code_line < minLine) minLine = p.code_line;
            if (p.code_line > maxLine) maxLine = p.code_line;
        }
        varIntervals[var].push_back({minLine, maxLine});
    }
    
    // Step 2: merge overlapping or touching intervals for each variable
    vector<Web> webs;
    int id = 0;
    for (auto &[var, intervals] : varIntervals) {
        // sort by start
        sort(intervals.begin(), intervals.end());
        vector<pair<int,int>> merged;
        for (auto &iv : intervals) {
            if (merged.empty() || iv.first > merged.back().second) {
                merged.push_back(iv);
            } else {
                merged.back().second = max(merged.back().second, iv.second);
            }
        }
        // create webs
        for (auto &iv : merged) {
            Web w;
            w.variable = var;
            w.start = iv.first;
            w.end = iv.second;
            w.id = id++;
            w.spilled = false;
            webs.push_back(w);
        }
    }
    return webs;
}

// ---------- buildInterferenceGraph ----------
InterferenceGraph buildInterferenceGraph(const vector<Web> &webs) {
    InterferenceGraph graph;
    graph.webs = webs;
    int n = webs.size();
    graph.adj.assign(n, vector<bool>(n, false));
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // Two webs interfere if their life intervals overlap,
            // with the exception that if one starts exactly where the other ends,
            // they do NOT interfere (definition/use point).
            int a_start = webs[i].start;
            int a_end = webs[i].end;
            int b_start = webs[j].start;
            int b_end = webs[j].end;
            
            // Check overlap: they interfere if max(start) < min(end)
            // but if max(start) == min(end), no interference (the point is a definion and last use).
            int maxStart = max(a_start, b_start);
            int minEnd = min(a_end, b_end);
            
            bool interfere = false;
            if (maxStart < minEnd) {
                interfere = true;
            }
            // If they are exactly adjacent (one starts at the other's end), no interference.
            // That case is already covered because maxStart == minEnd means maxStart < minEnd is false.
            
            if (interfere) {
                graph.adj[i][j] = graph.adj[j][i] = true;
            }
        }
    }
    return graph;
}

// ---------- greedy K-coloring (Figure 9) ----------
// Returns true if the graph (with current webs) can be colored with K colors.
// On success, colorMap[i] = color index 0..K-1.
static bool tryColorWithK(const InterferenceGraph &graph, int K, vector<int> &colorMap) {
    int n = graph.webs.size();
    vector<int> degree(n, 0);
    vector<bool> removed(n, false);
    vector<vector<bool>> remainingAdj = graph.adj;
    
    // Calculate initial degrees using only non-removed nodes
    auto recalcDegrees = [&]() {
        for (int i = 0; i < n; ++i) {
            degree[i] = 0;
            if (removed[i]) continue;
            for (int j = 0; j < n; ++j) {
                if (!removed[j] && remainingAdj[i][j])
                    degree[i]++;
            }
        }
    };
    
    recalcDegrees();
    vector<int> stack;  // removal order
    
    // Step 1: repeat until all nodes are removed
    while (true) {
        // Find a node with degree < K that is still in the graph
        int node = -1;
        for (int i = 0; i < n; ++i) {
            if (!removed[i] && degree[i] < K) {
                node = i;
                break;
            }
        }
        if (node == -1) {
            // Check if all nodes removed
            int countRemaining = 0;
            for (int i = 0; i < n; ++i)
                if (!removed[i]) countRemaining++;
            if (countRemaining == 0)
                break;
            else
                return false;   // cannot color with K
        } else {
            // Remove node: mark, push to stack, update neighbors' degrees
            removed[node] = true;
            stack.push_back(node);
            // decrease degree of all non-removed neighbors
            for (int u = 0; u < n; ++u) {
                if (!removed[u] && remainingAdj[node][u]) {
                    degree[u]--;
                }
            }
        }
    }
    
    // Step 2: reconstruct colors by popping from stack
    vector<int> colors(n, -1);
    for (int i = stack.size() - 1; i >= 0; --i) {
        int node = stack[i];
        // find the smallest color (0..K-1) not used by any already colored neighbor
        vector<bool> usedColors(K, false);
        for (int u = 0; u < n; ++u) {
            if (u != node && colors[u] != -1 && remainingAdj[node][u]) {
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
        if (chosen == -1) return false; // should not happen
        colors[node] = chosen;
    }
    
    colorMap = colors;
    return true;
}

// ---------- basicRegisterAllocation ----------
AllocationResult basicRegisterAllocation(const vector<Web> &webs, int maxRegisters) {
    AllocationResult result;
    result.success = false;
    result.usedRegisters = 0;
    result.webToRegister.assign(webs.size(), -1);  // default to memory
    
    if (webs.empty()) {
        result.success = true;
        return result;   // no webs, trivial success with 0 registers
    }
    
    // Build interference graph
    InterferenceGraph graph = buildInterferenceGraph(webs);
    
    // Try K from 1 up to maxRegisters
    for (int K = 1; K <= maxRegisters; ++K) {
        vector<int> colors;
        if (tryColorWithK(graph, K, colors)) {
            result.usedRegisters = K;
            result.webToRegister = colors;
            result.success = true;
            return result;
        }
    }
    
    // If no K worked up to maxRegisters, allocation fails
    return result;
}

// ---------- writeAllocationOutput ----------
void writeAllocationOutput(const AllocationResult &result, const string &outputFile) {
    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "Could not open output file: " << outputFile << endl;
        return;
    }
    
    if (!result.success) {
        // Failure case: 0 registers, all webs to memory
        out << "Number of used registers: 0" << endl;
        for (size_t i = 0; i < result.webToRegister.size(); ++i) {
            out << "Web" << i << " -> M" << endl;
        }
        cerr << "Allocation failed: cannot color the graph with the given registers." << endl;
    } else {
        out << "Number of used registers: " << result.usedRegisters << endl;
        for (size_t i = 0; i < result.webToRegister.size(); ++i) {
            int reg = result.webToRegister[i];
            string regName = (reg == -1) ? "M" : "r" + to_string(reg);
            out << "Web" << i << " -> " << regName << endl;
        }
    }
    out.close();
    cout << "Allocation result written to " << outputFile << endl;
}