#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>
#include "data_structures/Graph.h"

using namespace std;

/**
 * @brief Represents one program point of a variable live range.
 */
struct ProgramPoint {
    int code_line;
    bool isDefinition;
    bool isLastUse;
};

/**
 * @brief Represents the live range of a variable.
 */
struct LiveRange {
    string variable;
    vector<ProgramPoint> points;
};

/**
 * @brief Stores the register allocation settings.
 */
struct RegisterSettings {
    int registers;
    string algorithm;
    int param;
};

/**
 * @brief Stores all input data read from the input files.
 */
struct InputData {
    vector<LiveRange> liveRanges;
    RegisterSettings settings;
    string outputFile;
    bool hasErrors = false;
};

/**
 * @brief Represents a web created from live ranges.
 */
struct Web {
    string variable;
    int start;
    int end;
    int id;
    vector<ProgramPoint> points;
};

/**
 * @brief Represents the interference graph between webs.
 */
struct InterferenceGraph {
    Graph<int> graph;
    vector<Web> webs;
    vector<vector<bool>> adj;
};

/**
 * @brief Stores the result of the register allocation algorithm.
 */
struct AllocationResult {
    int usedRegisters;
    vector<int> webToRegister;
    bool success;
    vector<Web> webs;
    string algorithm = "basic";
};

#endif