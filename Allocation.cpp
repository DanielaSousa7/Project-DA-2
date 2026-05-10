#include "Allocation.h"
#include "WebBuilder.h"

#include "algorithms/basic/Basic.h"
#include "algorithms/spilling/Spilling.h"
#include "algorithms/splitting/Splitting.h"
#include "algorithms/free/Free.h"

#include <fstream>
#include <iostream>

using namespace std;

AllocationResult runRegisterAllocation(InputData data) {
    vector<Web> webs = buildWebs(data.liveRanges);
    AllocationResult result = allocateWithAlgorithm(webs, data.settings);
    result.webs = webs;
    return result;
}

AllocationResult allocateWithAlgorithm(vector<Web> webs, RegisterSettings settings) {
    if (settings.algorithm == "basic") {
        return basicRegisterAllocation(webs, settings.registers);
    }

    if (settings.algorithm == "spilling") {
        return spillingRegisterAllocation(webs, settings.registers, settings.param);
    }

    if (settings.algorithm == "splitting") {
        return splittingRegisterAllocation(webs, settings.registers, settings.param);
    }

    if (settings.algorithm == "free") {
        return freeRegisterAllocation(webs, settings.registers);
    }

    return basicRegisterAllocation(webs, settings.registers);
}
