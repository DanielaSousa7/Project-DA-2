#ifndef GRAPHCOLORING_H
#define GRAPHCOLORING_H

#include "Structs.h"
#include <vector>

using namespace std;

// Build webs by merging live ranges of the same variable that intersect or touch
vector<Web> buildWebs(const vector<LiveRange> &liveRanges);

// Build the interference graph
InterferenceGraph buildInterferenceGraph(const vector<Web> &webs);

// Basic greedy K-coloring (T2.1)
AllocationResult basicRegisterAllocation(const vector<Web> &webs, int maxRegisters);

// T2.2: Spilling – tries up to maxSpill spills to color with maxRegisters
AllocationResult spillingRegisterAllocation(const vector<Web> &webs, int maxRegisters, int maxSpill);

// Generic driver that picks the algorithm based on settings
AllocationResult allocateWithAlgorithm(const vector<Web> &webs, const RegisterSettings &settings);

// Write allocation result to output file
void writeAllocationOutput(const AllocationResult &result, const string &outputFile);

#endif