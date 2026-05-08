#include "GraphColoring.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <functional>
#include <numeric>
using namespace std;

// ---------- buildWebs ----------
vector<Web> buildWebs(const vector<LiveRange> &liveRanges) {
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

    vector<Web> webs;
    int id = 0;
    for (auto &[var, intervals] : varIntervals) {
        sort(intervals.begin(), intervals.end());
        vector<pair<int,int>> merged;
        for (auto &iv : intervals) {
            if (merged.empty() || iv.first > merged.back().second) {
                merged.push_back(iv);
            } else {
                merged.back().second = max(merged.back().second, iv.second);
            }
        }
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
            int a_start = webs[i].start;
            int a_end = webs[i].end;
            int b_start = webs[j].start;
            int b_end = webs[j].end;

            int maxStart = max(a_start, b_start);
            int minEnd = min(a_end, b_end);
            // Interfere if maxStart < minEnd.
            // Adjacent (maxStart == minEnd) means no interference.
            if (maxStart < minEnd) {
                graph.adj[i][j] = graph.adj[j][i] = true;
            }
        }
    }
    return graph;
}

// ---------- tryColorWithK (greedy removal) ----------
static bool tryColorWithK(const InterferenceGraph &graph, int K, vector<int> &colorMap) {
    int n = graph.webs.size();
    vector<int> degree(n, 0);
    vector<bool> removed(n, false);
    vector<vector<bool>> remainingAdj = graph.adj;

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
    vector<int> stack;

    // Remove nodes with degree < K iteratively
    while (true) {
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
            removed[node] = true;
            stack.push_back(node);
            for (int u = 0; u < n; ++u) {
                if (!removed[u] && remainingAdj[node][u]) {
                    degree[u]--;
                }
            }
        }
    }

    // Reconstruct colors
    vector<int> colors(n, -1);
    for (int i = stack.size() - 1; i >= 0; --i) {
        int node = stack[i];
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
        if (chosen == -1) return false;
        colors[node] = chosen;
    }
    colorMap = colors;
    return true;
}

// ---------- basicRegisterAllocation (T2.1) ----------
AllocationResult basicRegisterAllocation(const vector<Web> &webs, int maxRegisters) {
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
            result.usedRegisters = K;
            result.webToRegister = colors;
            result.success = true;
            return result;
        }
    }
    return result;
}

// ---------- spillingRegisterAllocation (T2.2) ----------
AllocationResult spillingRegisterAllocation(const vector<Web> &webs, int maxRegisters, int maxSpill) {
    // Try without spilling first
    AllocationResult res0 = basicRegisterAllocation(webs, maxRegisters);
    if (res0.success) return res0;

    int n = webs.size();
    // Try 1..maxSpill spills
    for (int spillCount = 1; spillCount <= maxSpill; ++spillCount) {
        // Generate all combinations of spillCount webs to spill
        vector<vector<int>> combos;
        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);

        function<void(int, int, vector<int>&)> backtrack = [&](int start, int left, vector<int>& current) {
            if (left == 0) {
                combos.push_back(current);
                return;
            }
            for (int i = start; i <= n - left; ++i) {
                current.push_back(indices[i]);
                backtrack(i + 1, left - 1, current);
                current.pop_back();
            }
        };

        vector<int> current;
        backtrack(0, spillCount, current);

        for (auto &spillSet : combos) {
            // Build sublist of non-spilled webs
            vector<Web> subWebs;
            vector<int> subToOrig;  // map sub index -> original index
            for (int i = 0; i < n; ++i) {
                if (find(spillSet.begin(), spillSet.end(), i) == spillSet.end()) {
                    subWebs.push_back(webs[i]);
                    subToOrig.push_back(i);
                }
            }
            // Reassign temporary ids
            for (size_t i = 0; i < subWebs.size(); ++i) {
                subWebs[i].id = i;
            }

            AllocationResult subRes = basicRegisterAllocation(subWebs, maxRegisters);
            if (subRes.success) {
                AllocationResult result;
                result.success = true;
                result.usedRegisters = subRes.usedRegisters;
                result.webToRegister.assign(n, -1);  // all M
                for (size_t i = 0; i < subWebs.size(); ++i) {
                    int origIdx = subToOrig[i];
                    result.webToRegister[origIdx] = subRes.webToRegister[i];
                }
                return result;
            }
        }
    }

    // Failure
    AllocationResult fail;
    fail.success = false;
    fail.usedRegisters = 0;
    fail.webToRegister.assign(n, -1);
    return fail;
}

// ---------- allocateWithAlgorithm (router) ----------
AllocationResult allocateWithAlgorithm(const vector<Web> &webs, const RegisterSettings &settings) {
    if (settings.algorithm == "basic") {
        return basicRegisterAllocation(webs, settings.registers);
    } else if (settings.algorithm == "spilling") {
        return spillingRegisterAllocation(webs, settings.registers, settings.param);
    }
    cerr << "Algorithm " << settings.algorithm << " not implemented. Falling back to basic." << endl;
    return basicRegisterAllocation(webs, settings.registers);
}

// ---------- writeAllocationOutput ----------
void writeAllocationOutput(const AllocationResult &result, const string &outputFile) {
    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "Could not open output file: " << outputFile << endl;
        return;
    }

    if (!result.success) {
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