#ifndef WEBBUILDER_H
#define WEBBUILDER_H

#include <vector>
#include "structs.h"
using namespace std;

/**
 * @brief Builds webs from live ranges.
 *
 * @param liveRanges Vector containing all live ranges.
 * @return Vector of created webs.
 *
 * @complexity O(n² * p).
 */
vector<Web> buildWebs(vector<LiveRange> liveRanges);

/**
 * @brief Builds the interference graph from webs.
 *
 * @param webs Vector containing all webs.
 * @return Interference graph.
 *
 * @complexity O(w²).
 */
InterferenceGraph buildInterferenceGraph(vector<Web> webs);

#endif