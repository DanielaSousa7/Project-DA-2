#ifndef SPLITTING_H
#define SPLITTING_H

#include <vector>
#include "../../structs.h"
using namespace std;

vector<vector<int>> cartesianProduct(vector<vector<int>> sets);
AllocationResult splittingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSplits);

#endif