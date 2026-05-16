#include "Output.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Writes the register allocation result to an output file.
 *
 * The file contains the list of webs, their program points and the register
 * assigned to each web. If allocation fails, all webs are assigned to memory.
 *
 * @param result Allocation result produced by the algorithm.
 * @param webs Vector containing the program webs.
 * @param outputFile Name of the output file.
 *
 * @complexity O(w * p), where w is the number of webs and p is the number of program points per web.
 */
void writeAllocationOutput(AllocationResult result, vector<Web> webs, string outputFile) {
    ofstream out(outputFile);

    if (!out.is_open()) {
        cerr << "Could not open output file: " << outputFile << endl;
        return;
    }
    out << "# Algorithm used and success status" << endl;
    out << "Algorithm: " << result.algorithm_used << endl;
    if (result.success) {
      out << "Register allocation: Successful\n" << endl;
    }
    else {
        out << "Register allocation: Failed\n" << endl;
    }

    out << "# All webs followed by listing of the program points present in each one\n";
    out << "# (program points in each web are sorted in ascending order)" << endl;

    if (result.splitInfo.size() != 0) out << "Web splitting:" << endl;
    for (const auto& split : result.splitInfo) {
        int origIdx = split.first;
        const auto& newIds = split.second;
        if (newIds.size() == 2) {
            out << "- original web" << origIdx << " was split into web" << newIds[0] << " and web" << newIds[1] << endl;
        } else if (newIds.size() == 1) {
            out << "- original web" << origIdx << " was split into web" << newIds[0] << endl;
        }
    }
    if (result.splitInfo.size() != 0) out << endl;

    out << "Total amount of webs: " << webs.size() << endl;

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

    out << endl << "# All registers used, followed by assignment to respective webs\n";
    out << "# (\"M\" stands for memory)" << endl;
    out << "Total amount of registers used: " << result.usedRegisters << endl;

    for (size_t i = 0; i < result.webToRegister.size(); ++i) {
        int reg = result.webToRegister[i];

        if (reg == -1) {
            out << "M: web" << i << endl;
        }
        else {
            out << "r" << reg << ": web" << i << endl;
        }
    }
    out.close();
}