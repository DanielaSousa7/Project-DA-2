#ifndef SPLITTING_H
#define SPLITTING_H

#include <vector>
#include "../../structs.h"

AllocationResult splittingRegisterAllocation(
    const std::vector<Web> &webs,
    int maxRegisters,
    int maxSplits
);

#endif