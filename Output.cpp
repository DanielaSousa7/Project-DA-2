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
    out << "# Total number of webs followed by the listing of the program points of each one\n";
    out << "# program points in each web are sorted in ascending order\n";
    out << "webs: " << webs.size() << endl;

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

    out << "# Total number of registers used, followed by assignment to webs\n";
    out << "algorithm used: " << result.algorithm_used << "\n";
    if (!result.success) {
        out << "registers: 0" << endl;

        for (size_t i = 0; i < webs.size(); ++i) {
            out << "M: web" << i << endl;
        }
        cerr << "Allocation failed: cannot color the graph with the given registers." << endl;
    }
    else {
        out << "registers: " << result.usedRegisters << endl;

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