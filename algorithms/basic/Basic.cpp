#include "Basic.h"
#include "../../WebBuilder.h"

#include <vector>

using namespace std;

static bool tryColorWithK(InterferenceGraph graph, int K, vector<int> colorMap) {
    int n = graph.webs.size();

    vector<int> degree(n, 0);
    vector<bool> removed(n, false);
    vector<vector<bool>> remainingAdj = graph.adj;
    vector<int> stack;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (remainingAdj[i][j]) degree[i]++;
        }
    }

    while (true) {
        int node = -1;
        for (int i = 0; i < n; ++i) {
            if (!removed[i] && degree[i] < K) {
                node = i;
                break;
            }
        }
        if (node == -1) {
            int remaining = 0;

            for (int i = 0; i < n; ++i) {
                if (!removed[i]) remaining++;
            }

            if (remaining == 0) break;
            return false;
        }
        removed[node] = true;
        stack.push_back(node);
        for (int u = 0; u < n; ++u) {
            if (!removed[u] && remainingAdj[node][u]) {
                degree[u]--;
            }
        }
    }

    vector<int> colors(n, -1);
    for (int i = stack.size() - 1; i >= 0; --i) {
        int node = stack[i];
        vector<bool> usedColors(K, false);

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
        if (chosen == -1) return false;
        colors[node] = chosen;
    }
    colorMap = colors;
    return true;
}

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