#ifndef SPILLING_H
#define SPILLING_H

#include <vector>
#include "../../structs.h"
using namespace std;

AllocationResult spillingRegisterAllocation(vector<Web> webs, int maxRegisters, int maxSpill);

#endif