/**
 * @file Spilling.cpp
 * @brief Implementation of web spilling register allocation.
 *
 * Uses brute-force search over subsets of webs to spill.
 */

#include "Spilling.h"
#include "../basic/Basic.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

/**
 * @brief Attempts allocation by spilling webs.
 *
 * @param webs Original webs.
 * @param maxRegisters Number of registers.
 * @param maxSpill Max number of webs to spill.
 * @return AllocationResult with mapping (spilled webs: -1).
 *
 * @complexity O( sum_{s=1..maxSpill} C(N,s) * N^2 ), where N is the number of webs.
 */
AllocationResult spillingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSpill) {
    AllocationResult basicResult = basicRegisterAllocation(webs, maxRegisters);
    if (basicResult.success) {
        return basicResult;
    }
    int n = webs.size();

    for (int spillCount = 1; spillCount <= maxSpill; ++spillCount) {
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
        generateCombinations(0, spillCount, current);

        for (auto spillSet : combos) {
            vector<Web> subWebs;
            vector<int> subToOrig;
            for (int i = 0; i < n; ++i) {
                if (find(spillSet.begin(), spillSet.end(), i) == spillSet.end()) {
                    subWebs.push_back(webs[i]);
                    subToOrig.push_back(i);
                }
            }
            for (size_t i = 0; i < subWebs.size(); ++i) {
                subWebs[i].id = i;
            }

            AllocationResult subResult = basicRegisterAllocation(subWebs, maxRegisters);
            if (subResult.success) {
                AllocationResult result;
                result.algorithm_used = "spilling";
                result.success = true;
                result.usedRegisters = subResult.usedRegisters;
                result.webToRegister.assign(n, -1);
                for (size_t i = 0; i < subWebs.size(); ++i) {
                    int originalIndex = subToOrig[i];
                    result.webToRegister[originalIndex] = subResult.webToRegister[i];
                }
                result.webs = webs;
                return result;
            }
        }
    }

    AllocationResult fail;
    fail.success = false;
    fail.usedRegisters = 0;
    fail.webToRegister.assign(n, -1);
    fail.webs = webs;
    fail.algorithm_used = "spilling";
    return fail;
}