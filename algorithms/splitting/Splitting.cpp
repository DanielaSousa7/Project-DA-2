/**
 * @file Splitting.cpp
 * @brief Implementation of web splitting for register allocation.
 *
 * Implements a brute-force search over subsets of webs to split and possible
 * split points, then attempts basic allocation on the expanded web set.
 */

#include "Splitting.h"
#include "../basic/Basic.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

/**
 * @brief Generates Cartesian product of given integer sets.
 *
 * @param sets Sets of possible split points for each chosen web.
 * @return All combinations.
 *
 * @complexity O(P * S), where P is the total number of combinations and S is the number of sets.
 */
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

/**
 * @brief Attempts allocation with web splitting.
 *
 * @param webs Original webs.
 * @param maxRegisters Number of registers.
 * @param maxSplits Max webs to split.
 * @return AllocationResult.
 *
 * @complexity O( sum_{s=1..maxSplits} C(N,s) * R^s * (N+s)^2 ), where N is the number of webs and R is the average number of possible split points per web.
 */
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
                for (int i = 0; i < n; ++i) {
                    auto it = find(splitSet.begin(), splitSet.end(), i);
                    if (it == splitSet.end()) {
                        Web w = webs[i];
                        w.id = newWebs.size();
                        newWebs.push_back(w);
                    }
                    else {
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
                AllocationResult subResult = basicRegisterAllocation(newWebs, maxRegisters);
                if (subResult.success) {
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