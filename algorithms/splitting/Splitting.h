#ifndef SPLITTING_H
#define SPLITTING_H

#include <vector>
#include "../../structs.h"
using namespace std;

AllocationResult splittingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSplits);

#endif