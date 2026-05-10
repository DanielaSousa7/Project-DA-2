#ifndef FREE_H
#define FREE_H

#include <vector>
#include "../../structs.h"
using namespace  std;

bool tryDSATUR(InterferenceGraph graph, int K, vector<int>& colorMap);
AllocationResult freeRegisterAllocation(vector<Web> webs, int maxRegisters);

#endif