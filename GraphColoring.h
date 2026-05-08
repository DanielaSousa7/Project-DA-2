#ifndef GRAPHCOLORING_H
#define GRAPHCOLORING_H

#include "Structs.h"
#include <vector>

using namespace std;

vector<Web> buildWebs(const vector<LiveRange> &liveRanges);
InterferenceGraph buildInterferenceGraph(const vector<Web> &webs);
AllocationResult basicRegisterAllocation(const vector<Web> &webs, int maxRegisters);
AllocationResult spillingRegisterAllocation(const vector<Web> &webs, int maxRegisters, int maxSpill);
AllocationResult splittingRegisterAllocation(const vector<Web> &webs, int maxRegisters, int maxSplits);
AllocationResult allocateWithAlgorithm(const vector<Web> &webs, const RegisterSettings &settings);
void writeAllocationOutput(const AllocationResult &result, const string &outputFile);

#endif