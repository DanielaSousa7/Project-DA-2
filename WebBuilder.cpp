#include "WebBuilder.h"
#include <algorithm>
#include <map>

using namespace std;

vector<Web> buildWebs(vector<LiveRange> liveRanges) {
    map<string, vector<LiveRange>> varRanges;
    for (auto lr : liveRanges) {
        if (!lr.points.empty()) {
            varRanges[lr.variable].push_back(lr);
        }
    }
    vector<Web> webs;
    int id = 0;

    for (auto &[var, ranges] : varRanges) {
        sort(ranges.begin(), ranges.end(), [](LiveRange a, LiveRange b) {
            return a.points.front().code_line < b.points.front().code_line;
        });

        vector<Web> tempWebs;

        for (auto lr : ranges) {
            int minLine = lr.points[0].code_line;
            int maxLine = lr.points[0].code_line;

            for (auto p : lr.points) {
                minLine = min(minLine, p.code_line);
                maxLine = max(maxLine, p.code_line);
            }
            bool merged = false;

            for (auto w : tempWebs) {
                if (minLine <= w.end && maxLine >= w.start) {
                    w.start = min(w.start, minLine);
                    w.end = max(w.end, maxLine);

                    for (auto p : lr.points) {
                        w.points.push_back(p);
                    }
                    merged = true;
                    break;
                }
            }
            if (!merged) {
                Web w;
                w.variable = var;
                w.start = minLine;
                w.end = maxLine;
                w.id = -1;
                w.spilled = false;
                w.points = lr.points;
                tempWebs.push_back(w);
            }
        }

        for (auto w : tempWebs) {
            sort(w.points.begin(), w.points.end(), [](ProgramPoint a, ProgramPoint b) {
                return a.code_line < b.code_line;
            });

            vector<ProgramPoint> uniquePoints;
            for (auto p : w.points) {
                bool exists = false;

                for (auto up : uniquePoints) {
                    if (up.code_line == p.code_line) {
                        up.isDefinition = up.isDefinition || p.isDefinition;
                        up.isLastUse = up.isLastUse || p.isLastUse;
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    uniquePoints.push_back(p);
                }
            }
            w.points = uniquePoints;
            w.id = id++;
            webs.push_back(w);
        }
    }
    return webs;
}

InterferenceGraph buildInterferenceGraph(vector<Web> webs) {
    InterferenceGraph graph;
    graph.webs = webs;
    int n = webs.size();
    graph.adj.assign(n, vector<bool>(n, false));

    // Add vertices to the TP Graph structure
    for (Web w : webs) {
        graph.graph.addVertex(w.id);
    }
    // Add interference edges
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int maxStart = max(webs[i].start, webs[j].start);
            int minEnd = min(webs[i].end, webs[j].end);

            if (maxStart < minEnd) {
                graph.adj[i][j] = true;
                graph.adj[j][i] = true;
                graph.graph.addBidirectionalEdge(webs[i].id, webs[j].id, 1);
            }
        }
    }
    return graph;
}