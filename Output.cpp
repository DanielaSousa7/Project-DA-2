#include "Output.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void writeAllocationOutput(AllocationResult result, vector<Web> webs, string outputFile) {
    ofstream out(outputFile);

    if (!out.is_open()) {
        cerr << "Could not open output file: " << outputFile << endl;
        return;
    }
    out << "# All webs followed by listing of the program points present in each one\n";
    out << "# (program points in each web are sorted in ascending order)\n" << endl;
    out << "Algorithm used: " << result.algorithm_used << endl;


    if (result.splitInfo.size() != 0) out << "Web splitting:" << endl;
    for (const auto& split : result.splitInfo) {
        int origIdx = split.first;
        const auto& newIds = split.second;
        if (newIds.size() == 2) {
            out << "- web" << origIdx << " was split into web" << newIds[0] << " and web" << newIds[1] << endl;
        } else if (newIds.size() == 1) {
            out << "- web" << origIdx << " was split into web" << newIds[0] << endl;
        }
    }

    out << endl << "Total amount of webs: " << webs.size() << endl;

    for (size_t i = 0; i < webs.size(); ++i) {
        out << "web" << i << ": ";

        for (size_t j = 0; j < webs[i].points.size(); ++j) {
            ProgramPoint p = webs[i].points[j];

            out << p.code_line;

            if (p.isDefinition && !p.isLastUse) {
                out << "+";
            }
            else if (p.isLastUse && !p.isDefinition) {
                out << "-";
            }

            if (j + 1 < webs[i].points.size()) {
                out << ",";
            }
        }
        out << endl;
    }

    out << endl << "# Total number of registers used, followed by assignment to respective webs\n";
    if (!result.success) {
        out << "Total amount of registers: 0" << endl;

        for (size_t i = 0; i < webs.size(); ++i) {
            out << "M: web" << i << endl;
        }
        cerr << "Allocation failed: cannot color the graph with the given registers." << endl;
    }
    else {
        out << "Total amount of registers: " << result.usedRegisters << endl;

        for (size_t i = 0; i < result.webToRegister.size(); ++i) {
            int reg = result.webToRegister[i];

            if (reg == -1) {
                out << "M: web" << i << endl;
            }
            else {
                out << "r" << reg << ": web" << i << endl;
            }
        }
    }
    out.close();
}