#include "Parser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
static vector<string> errors;

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

ProgramPoint parseProgramPoint(string token) {
    ProgramPoint point;
    token = trim(token);
    point.code_line = -1;
    point.isDefinition = false;
    point.isLastUse = false;
    if (token.empty()) {
        errors.push_back("Empty program point.");
        return point;
    }
    char lastChar = token[token.size() - 1];
    if (lastChar == '+') {
        point.isDefinition = true;
        token.pop_back();
    }
    else if (lastChar == '-') {
        point.isLastUse = true;
        token.pop_back();
    }
    token = trim(token);
    point.code_line = stoi(token);
    return point;
}

vector<LiveRange> parseLiveRangesFile(string filename) {
    vector<LiveRange> liveRanges;
    ifstream file(filename);
    if (!file.is_open()) {
        errors.push_back("Could not open live ranges file: " + filename);
        return liveRanges;
    }
    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        size_t colonPos = line.find(':');
        if (colonPos == string::npos) {
            errors.push_back("Invalid live range line: " + line);
            continue;
        }
        string variable = trim(line.substr(0, colonPos));
        string pointsText = trim(line.substr(colonPos + 1));
        if (variable.empty()) {
            errors.push_back("Missing variable name in line: " + line);
            continue;
        }
        if (pointsText.empty()) {
            errors.push_back("Missing live range points in line: " + line);
            continue;
        }
        LiveRange range;
        range.variable = variable;
        stringstream ss(pointsText);
        string token;
        while (getline(ss, token, ',')) {
            ProgramPoint point = parseProgramPoint(token);
            if (point.code_line != -1) {
                range.points.push_back(point);
            }
        }
        if (!range.points.empty()) {
            liveRanges.push_back(range);
        }
    }
    file.close();
    return liveRanges;
}

RegisterSettings parseRegistersFile(string filename) {
    RegisterSettings settings;
    settings.registers = 0;
    settings.algorithm = "";
    ifstream file(filename);
    if (!file.is_open()) {
        errors.push_back("Could not open registers file: " + filename);
        return settings;
    }
    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        size_t colonPos = line.find(':');
        if (colonPos == string::npos) {
            errors.push_back("Invalid settings line: " + line);
            continue;
        }
        string key = trim(line.substr(0, colonPos));
        string value = trim(line.substr(colonPos + 1));
        if (key == "registers") {
            settings.registers = stoi(value);
        }
        else if (key == "algorithm") {
            if (value == "basic" || value == "splitting" || value == "spilling") {
                settings.algorithm = value;
            }
            else {
                errors.push_back("Invalid algorithm: " + value);
            }
        }
        else {
            errors.push_back("Unknown settings key: " + key);
        }
    }
    file.close();
    if (settings.registers <= 0) {
        errors.push_back("Number of registers must be greater than 0.");
    }
    if (settings.algorithm.empty()) {
        errors.push_back("Algorithm not defined.");
    }
    return settings;
}

InputData parseInputFiles(string rangesFile, string registersFile, string outputFile) {
    InputData data;
    errors.clear();
    data.liveRanges = parseLiveRangesFile(rangesFile);
    data.settings = parseRegistersFile(registersFile);
    data.outputFile = outputFile;
    data.hasErrors = false;
    if (data.outputFile.empty()) {
        data.outputFile = "output.txt";
    }
    if (!errors.empty()) {
        data.hasErrors = true;
        cout << "Errors while reading input files:" << endl;
        for (string error : errors) {
            cout << "- " << error << endl;
        }
    }
    return data;
}