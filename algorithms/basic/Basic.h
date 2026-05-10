#ifndef BASIC_H
#define BASIC_H

#include <vector>
#include "../../structs.h"
using namespace std;

bool tryColorWithK(InterferenceGraph graph, int K, vector<int>& colorMap);
AllocationResult basicRegisterAllocation(vector<Web> webs, int maxRegisters);

#endif