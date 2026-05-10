#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <string>
#include <vector>
#include "structs.h"
using namespace  std;

AllocationResult runRegisterAllocation(InputData data);
AllocationResult allocateWithAlgorithm(vector<Web> webs, RegisterSettings settings);

#endif