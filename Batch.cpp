#include "Batch.h"
#include "Parser.h"
#include "Allocation.h"
#include "Output.h"
#include <iostream>
using namespace std;

void batchMode(string rangesFile, string registersFile, string outputFile) {
    InputData data = parseInputFiles(rangesFile, registersFile, outputFile);
    if (data.hasErrors) {
        cerr << "Errors found while reading input files. Aborting batch mode." << endl;
        return;
    }

    cout << "Loaded: " << data.liveRanges.size() << " live range(s), "<< data.settings.registers << " register(s), algorithm: "<< data.settings.algorithm;
    if (!data.settings.algorithm.empty() && data.settings.param > 0)
        cout << ", " << data.settings.param;
    cout << endl;

    AllocationResult result = runRegisterAllocation(data);
    writeAllocationOutput(result, data.outputFile);

    cout << "[Output written to: " << outputFile << "]\n";
}