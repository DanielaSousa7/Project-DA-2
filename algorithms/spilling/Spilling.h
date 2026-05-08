#ifndef SPILLING_H
#define SPILLING_H

#include <vector>
#include "../../structs.h"

AllocationResult spillingRegisterAllocation(
    const std::vector<Web> &webs,
    int maxRegisters,
    int maxSpill
);

#endif