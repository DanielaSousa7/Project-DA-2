#ifndef PARSER_H
#define PARSER_H

#include "Structs.h"
#include <string>

using namespace std;

string trim(string s);
ProgramPoint parseProgramPoint(string token);
vector<LiveRange> parseLiveRangesFile(string filename);
RegisterSettings parseRegistersFile(string filename);
InputData parseInputFiles(string rangesFile, string registersFile, string outputFile);

#endif