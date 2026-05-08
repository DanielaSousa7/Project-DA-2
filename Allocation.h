#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <string>
#include <vector>
#include "structs.h"

AllocationResult runRegisterAllocation(const InputData data);
AllocationResult allocateWithAlgorithm(const std::vector<Web> &webs, const RegisterSettings &settings);

#endif