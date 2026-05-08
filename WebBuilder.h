#ifndef WEBBUILDER_H
#define WEBBUILDER_H

#include <vector>
#include "structs.h"
using namespace std;

vector<Web> buildWebs(vector<LiveRange> liveRanges);

InterferenceGraph buildInterferenceGraph(const std::vector<Web>& webs);

#endif