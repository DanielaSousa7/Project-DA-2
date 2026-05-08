#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <string>
#include <vector>
#include "structs.h"

AllocationResult allocateWithAlgorithm(
    const std::vector<Web> &webs,
    const RegisterSettings &settings
);

void writeAllocationOutput(
    const AllocationResult &result,
    const std::string &outputFile
);

#endif