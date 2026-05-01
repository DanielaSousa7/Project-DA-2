#ifndef GRAPHCOLORING_H
#define GRAPHCOLORING_H

#include "Structs.h"
#include <vector>

using namespace std;

// Build webs by merging all live ranges of the same variable that intersect or touch
vector<Web> buildWebs(const vector<LiveRange> &liveRanges);

// Build the interference graph from the webs
InterferenceGraph buildInterferenceGraph(const vector<Web> &webs);

// Greedy K-coloring algorithm (Figure 9). Returns a valid color assignment if possible.
AllocationResult basicRegisterAllocation(const vector<Web> &webs, int maxRegisters);

// Utility to write the output file
void writeAllocationOutput(const AllocationResult &result, const string &outputFile);

#endif