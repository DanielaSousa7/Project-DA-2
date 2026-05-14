/**
 * @file Batch.cpp
 * @brief Implementation of batch-mode processing.
 *
 * Implements the batchMode() function that orchestrates reading, allocating,
 * and writing results without user interaction.
 */

#include "Batch.h"
#include "Parser.h"
#include "Allocation.h"
#include "Output.h"
#include <iostream>
using namespace std;

/**
 * @brief Executes batch mode: parse, allocate, output.
 *
 * Steps:
 * 1. Parse input files.
 * 2. If parsing errors occur, report and abort.
 * 3. Print summary of loaded data to console.
 * 4. Run register allocation.
 * 5. Write results to output file.
 *
 * @param rangesFile Live ranges input file.
 * @param registersFile Register settings input file.
 * @param outputFile Destination file for allocation mapping.
 *
 * @complexity O(n + m + A) where:
 *             n = total lines in ranges file,
 *             m = lines in registers file,
 *             A = complexity of the allocation algorithm (see Allocation.cpp).
 *             All file I/O is linear in input size.
 */
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
    writeAllocationOutput(result, result.webs, data.outputFile);

    cout << "[Output written to: " << outputFile << "]\n";
}