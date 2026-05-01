#ifndef PARSER_H
#define PARSER_H

#include "Structs.h"
#include <string>

using namespace std;

string trim(string s);

InputData parseInputFiles(string rangesFile, string registersFile, string outputFile);

vector<LiveRange> parseLiveRangesFile(string filename, vector<string>& errors);

RegisterSettings parseRegistersFile(string filename, vector<string>& errors);

ProgramPoint parseProgramPoint(string token, vector<string>& errors);

#endif