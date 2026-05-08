#include "Allocation.h"

#include "algorithms/basic/Basic.h"
#include "algorithms/spilling/Spilling.h"
#include "algorithms/splitting/Splitting.h"
#include "algorithms/free/Free.h"

#include <fstream>
#include <iostream>

using namespace std;

AllocationResult allocateWithAlgorithm(const vector<Web> &webs, const RegisterSettings &settings) {
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

    cerr << "Algorithm " << settings.algorithm << " not implemented. Falling back to basic." << endl;

    return basicRegisterAllocation(webs, settings.registers);
}
