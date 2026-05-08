#include "WebBuilder.h"
#include <algorithm>
#include <map>

using namespace std;

vector<Web> buildWebs(vector<LiveRange> liveRanges) {
    map<string, vector<pair<int, int>>> varIntervals;

    for (const auto &lr : liveRanges) {
        if (lr.points.empty()) continue;

        string var = lr.variable;
        int minLine = lr.points[0].code_line;
        int maxLine = lr.points[0].code_line;

        for (const auto &p : lr.points) {
            minLine = min(minLine, p.code_line);
            maxLine = max(maxLine, p.code_line);
        }

        varIntervals[var].push_back({minLine, maxLine});
    }

    vector<Web> webs;
    int id = 0;

    for (auto &[var, intervals] : varIntervals) {
        sort(intervals.begin(), intervals.end());

        vector<pair<int, int>> merged;

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
            int maxStart = max(webs[i].start, webs[j].start);
            int minEnd = min(webs[i].end, webs[j].end);

            if (maxStart < minEnd) {
                graph.adj[i][j] = true;
                graph.adj[j][i] = true;
            }
        }
    }

    return graph;
}