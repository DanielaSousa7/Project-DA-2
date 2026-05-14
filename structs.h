#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>
#include "data_structures/Graph.h"

using namespace std;

struct ProgramPoint {
    int code_line;
    bool isDefinition;
    bool isLastUse;
};

struct LiveRange {
    string variable;
    vector<ProgramPoint> points;
};

struct RegisterSettings {
    int registers;
    string algorithm;
    int param;
};

struct InputData {
    vector<LiveRange> liveRanges;
    RegisterSettings settings;
    string outputFile;
    bool hasErrors = false;
};

struct Web {
    string variable;      // original variable name
    int start;            // first program line where this web is alive
    int end;              // last program line where this web is alive
    int id;               // unique identifier (0..n-1)
    vector<ProgramPoint> points;
};

struct InterferenceGraph {
    Graph<int> graph;              // main graph structure from TP classes
    vector<Web> webs;
    vector<vector<bool>> adj;   // adjacency matrix: adj[i][j] true if web i and web j interfere
};

struct AllocationResult {
    int usedRegisters;           // number of physical registers successfully used (0 if failure)
    vector<int> webToRegister;   // map web id -> register number (0..K-1), or -1 for memory
    bool success;
    vector<Web> webs;
    string algorithm_used = "basic";  // initialize as "basic"
};

#endif