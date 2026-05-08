#ifndef BASIC_H
#define BASIC_H

#include <vector>
#include "../../structs.h"

AllocationResult basicRegisterAllocation(const std::vector<Web> &webs, int maxRegisters);

#endif