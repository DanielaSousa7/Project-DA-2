#include "GraphColoring.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <functional>
#include <numeric>
using namespace std;

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
            if (maxStart < minEnd) {
                graph.adj[i][j] = graph.adj[j][i] = true;
            }
        }
    }
    return graph;
}

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

    while (true) {
        int node = -1;
        for (int i = 0; i < n; ++i) {
            if (!removed[i] && degree[i] < K) {
                node = i;
                break;
            }
        }
        if (node == -1) {
            int countRemaining = 0;
            for (int i = 0; i < n; ++i)
                if (!removed[i]) countRemaining++;
            if (countRemaining == 0) break;
            else return false;
        } else {
            removed[node] = true;
            stack.push_back(node);
            for (int u = 0; u < n; ++u) {
                if (!removed[u] && remainingAdj[node][u])
                    degree[u]--;
            }
        }
    }

    vector<int> colors(n, -1);
    for (int i = stack.size() - 1; i >= 0; --i) {
        int node = stack[i];
        vector<bool> usedColors(K, false);
        for (int u = 0; u < n; ++u) {
            if (u != node && colors[u] != -1 && remainingAdj[node][u])
                usedColors[colors[u]] = true;
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

AllocationResult spillingRegisterAllocation(const vector<Web> &webs, int maxRegisters, int maxSpill) {
    AllocationResult res0 = basicRegisterAllocation(webs, maxRegisters);
    if (res0.success) return res0;

    int n = webs.size();
    for (int spillCount = 1; spillCount <= maxSpill; ++spillCount) {
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
            vector<Web> subWebs;
            vector<int> subToOrig;
            for (int i = 0; i < n; ++i) {
                if (find(spillSet.begin(), spillSet.end(), i) == spillSet.end()) {
                    subWebs.push_back(webs[i]);
                    subToOrig.push_back(i);
                }
            }
            for (size_t i = 0; i < subWebs.size(); ++i)
                subWebs[i].id = i;

            AllocationResult subRes = basicRegisterAllocation(subWebs, maxRegisters);
            if (subRes.success) {
                AllocationResult result;
                result.success = true;
                result.usedRegisters = subRes.usedRegisters;
                result.webToRegister.assign(n, -1);
                for (size_t i = 0; i < subWebs.size(); ++i) {
                    int origIdx = subToOrig[i];
                    result.webToRegister[origIdx] = subRes.webToRegister[i];
                }
                return result;
            }
        }
    }
    AllocationResult fail;
    fail.success = false;
    fail.usedRegisters = 0;
    fail.webToRegister.assign(n, -1);
    return fail;
}

static vector<vector<int>> cartesianProduct(const vector<vector<int>>& sets) {
    if (sets.empty()) return {{}};
    vector<vector<int>> result;
    vector<int> indices(sets.size(), 0);
    while (true) {
        vector<int> tuple;
        for (size_t i = 0; i < sets.size(); ++i)
            tuple.push_back(sets[i][indices[i]]);
        result.push_back(tuple);
        int pos = sets.size() - 1;
        while (pos >= 0 && indices[pos] + 1 == (int)sets[pos].size()) {
            indices[pos] = 0;
            --pos;
        }
        if (pos < 0) break;
        ++indices[pos];
    }
    return result;
}

AllocationResult splittingRegisterAllocation(const vector<Web> &webs, int maxRegisters, int maxSplits) {
    AllocationResult res0 = basicRegisterAllocation(webs, maxRegisters);
    if (res0.success) return res0;

    int n = webs.size();

    for (int splitCount = 1; splitCount <= maxSplits; ++splitCount) {
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
        backtrack(0, splitCount, current);

        for (auto &splitSet : combos) {
            vector<vector<int>> possiblePoints(splitSet.size());
            for (size_t i = 0; i < splitSet.size(); ++i) {
                int idx = splitSet[i];
                const Web &w = webs[idx];
                for (int p = w.start + 1; p <= w.end; ++p) {
                    possiblePoints[i].push_back(p);
                }
            }

            vector<vector<int>> pointCombos = cartesianProduct(possiblePoints);
            for (auto &points : pointCombos) {
                vector<Web> newWebs;
                for (int i = 0; i < n; ++i) {
                    auto it = find(splitSet.begin(), splitSet.end(), i);
                    if (it == splitSet.end()) {
                        Web w = webs[i];
                        w.id = newWebs.size();
                        newWebs.push_back(w);
                    } else {
                        int pos = distance(splitSet.begin(), it);
                        int splitPoint = points[pos];
                        if (splitPoint - 1 >= webs[i].start) {
                            Web w1;
                            w1.variable = webs[i].variable;
                            w1.start = webs[i].start;
                            w1.end = splitPoint - 1;
                            w1.id = newWebs.size();
                            newWebs.push_back(w1);
                        }
                        Web w2;
                        w2.variable = webs[i].variable;
                        w2.start = splitPoint;
                        w2.end = webs[i].end;
                        w2.id = newWebs.size();
                        newWebs.push_back(w2);
                    }
                }

                AllocationResult subRes = basicRegisterAllocation(newWebs, maxRegisters);
                if (subRes.success) {
                    subRes.webToRegister.resize(newWebs.size());
                    return subRes;
                }
            }
        }
    }

    AllocationResult fail;
    fail.success = false;
    fail.usedRegisters = 0;
    fail.webToRegister.assign(n, -1);
    return fail;
}

static bool tryDSATUR(const InterferenceGraph &graph, int K, vector<int> &colorMap) {
    int n = graph.webs.size();
    vector<int> colors(n, -1);
    vector<int> degree(n, 0);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (graph.adj[i][j]) degree[i]++;

    vector<set<int>> adjColors(n);

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

AllocationResult freeRegisterAllocation(const vector<Web> &webs, int maxRegisters) {
    int n = webs.size();
    vector<Web> currentWebs = webs;
    vector<int> originalIdx(n);
    iota(originalIdx.begin(), originalIdx.end(), 0);

    while (true) {
        vector<Web> subWebs = currentWebs;
        vector<int> subToOrig = originalIdx;

        if (!subWebs.empty()) {
            InterferenceGraph graph = buildInterferenceGraph(subWebs);
            vector<int> colors;
            bool ok = tryDSATUR(graph, maxRegisters, colors);
            if (ok) {
                AllocationResult result;
                result.success = true;
                result.usedRegisters = maxRegisters;
                result.webToRegister.assign(n, -1);
                for (size_t i = 0; i < subWebs.size(); ++i) {
                    int orig = subToOrig[i];
                    result.webToRegister[orig] = colors[i];
                }
                return result;
            }
        }

        InterferenceGraph currentGraph = buildInterferenceGraph(currentWebs);
        int spillIdx = -1;
        int maxDegree = -1;
        for (size_t i = 0; i < currentWebs.size(); ++i) {
            int d = 0;
            for (size_t j = 0; j < currentWebs.size(); ++j)
                if (currentGraph.adj[i][j]) d++;
            if (d > maxDegree) {
                maxDegree = d;
                spillIdx = i;
            }
        }
        if (spillIdx == -1) spillIdx = 0;

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

AllocationResult allocateWithAlgorithm(const vector<Web> &webs, const RegisterSettings &settings) {
    if (settings.algorithm == "basic") {
        return basicRegisterAllocation(webs, settings.registers);
    } else if (settings.algorithm == "spilling") {
        return spillingRegisterAllocation(webs, settings.registers, settings.param);
    } else if (settings.algorithm == "splitting") {
        return splittingRegisterAllocation(webs, settings.registers, settings.param);
    } else if (settings.algorithm == "free") {
        return freeRegisterAllocation(webs, settings.registers);
    }
    cerr << "Algorithm " << settings.algorithm << " not implemented. Falling back to basic." << endl;
    return basicRegisterAllocation(webs, settings.registers);
}