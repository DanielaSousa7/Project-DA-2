#include "Splitting.h"
#include "../basic/Basic.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

vector<vector<int>> cartesianProduct(vector<vector<int>> sets) {
    if (sets.empty()) return {{}};
    vector<vector<int>> result;
    vector<int> indices(sets.size(), 0);

    while (true) {
        vector<int> tuple;

        for (size_t i = 0; i < sets.size(); ++i) {
            tuple.push_back(sets[i][indices[i]]);
        }

        result.push_back(tuple);
        int pos = sets.size() - 1;

        while (pos >= 0 && indices[pos] + 1 == (int)sets[pos].size()) {
            indices[pos] = 0;
            --pos;
        }

        if (pos < 0) break;
        indices[pos]++;
    }
    return result;
}

AllocationResult splittingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSplits) {
    AllocationResult basicResult = basicRegisterAllocation(webs, maxRegisters);
    if (basicResult.success) {
        return basicResult;
    }
    int n = webs.size();

    for (int splitCount = 1; splitCount <= maxSplits; ++splitCount) {
        vector<vector<int>> combos;
        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);

        function<void(int, int, vector<int>&)> generateCombinations =
            [&](int start, int left, vector<int> &current) {
                if (left == 0) {
                    combos.push_back(current);
                    return;
                }

                for (int i = start; i <= n - left; ++i) {
                    current.push_back(indices[i]);
                    generateCombinations(i + 1, left - 1, current);
                    current.pop_back();
                }
            };

        vector<int> current;
        generateCombinations(0, splitCount, current);

        for (auto splitSet : combos) {
            vector<vector<int>> possiblePoints(splitSet.size());
            for (size_t i = 0; i < splitSet.size(); ++i) {
                int idx = splitSet[i];
                const Web &w = webs[idx];

                for (int p = w.start + 1; p <= w.end; ++p) {
                    possiblePoints[i].push_back(p);
                }
            }

            vector<vector<int>> pointCombos = cartesianProduct(possiblePoints);

            for (auto points : pointCombos) {
                vector<Web> newWebs;
                vector<pair<int, vector<int>>> splitInfo;
                auto filterPoints = [](const Web& orig, int start, int end) {
                    vector<ProgramPoint> pts;
                    for (const auto& p : orig.points) {
                        if (p.code_line >= start && p.code_line <= end) {
                            pts.push_back(p);
                        }
                    }
                    return pts;
                };

                for (int i = 0; i < n; ++i) {
                    auto it = find(splitSet.begin(), splitSet.end(), i);
                    if (it == splitSet.end()) {
                        // Unsplit web: copy whole web
                        Web w = webs[i];
                        w.id = newWebs.size();
                        newWebs.push_back(w);
                    } else {
                        int pos = distance(splitSet.begin(), it);
                        int splitPoint = points[pos];
                        vector<int> newIds;

                        // First part (if non‑empty)
                        if (splitPoint - 1 >= webs[i].start) {
                            Web w1;
                            w1.variable = webs[i].variable;
                            w1.start = webs[i].start;
                            w1.end = splitPoint - 1;
                            w1.points = filterPoints(webs[i], w1.start, w1.end);
                            w1.id = newWebs.size();
                            newWebs.push_back(w1);
                            newIds.push_back(w1.id);
                        }

                        // Second part (always non‑empty because splitPoint ≤ webs[i].end)
                        Web w2;
                        w2.variable = webs[i].variable;
                        w2.start = splitPoint;
                        w2.end = webs[i].end;
                        w2.points = filterPoints(webs[i], w2.start, w2.end);
                        w2.id = newWebs.size();
                        newWebs.push_back(w2);
                        newIds.push_back(w2.id);

                        splitInfo.push_back({i, newIds});
                    }
                }

                AllocationResult subResult = basicRegisterAllocation(newWebs, maxRegisters);
                if (subResult.success) {
                    subResult.algorithm_used = "splitting";
                    subResult.webs = newWebs;
                    subResult.splitInfo = splitInfo;
                    return subResult;
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