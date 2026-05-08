#ifndef FREE_H
#define FREE_H

#include <vector>
#include "../../structs.h"

AllocationResult freeRegisterAllocation(const std::vector<Web> &webs, int maxRegisters);

#endif