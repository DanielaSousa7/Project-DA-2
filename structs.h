#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>

using namespace std;

struct ProgramPoint {
    int line;
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
};

struct InputData {
    vector<LiveRange> liveRanges;
    RegisterSettings settings;
    string outputFile;
    bool hasErrors = false;
};

#endif