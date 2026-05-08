#include "Output.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void writeAllocationOutput(const AllocationResult &result, const string &outputFile) {
    ofstream out(outputFile);

    if (!out.is_open()) {
        cerr << "Could not open output file: " << outputFile << endl;
        return;
    }

    if (!result.success) {
        out << "Number of used registers: 0" << endl;

        for (size_t i = 0; i < result.webToRegister.size(); ++i) {
            out << "Web" << i << " -> M" << endl;
        }

        cerr << "Allocation failed: cannot color the graph with the given registers." << endl;
    } else {
        out << "Number of used registers: " << result.usedRegisters << endl;

        for (size_t i = 0; i < result.webToRegister.size(); ++i) {
            int reg = result.webToRegister[i];
            string regName = (reg == -1) ? "M" : "r" + to_string(reg);

            out << "Web" << i << " -> " << regName << endl;
        }
    }

    out.close();
    cout << "Allocation result written to " << outputFile << endl;
}