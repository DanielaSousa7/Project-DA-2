#include "Parser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
static vector<string> errors;

/**
 * @brief Removes leading and trailing whitespace from a string.
 *
 * @param s Input string.
 * @return Trimmed string.
 *
 * @complexity O(n), where n is the string length.
 */
string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

/**
 * @brief Parses a program point token.
 *
 * Detects if the point is a definition or last use and extracts the code line.
 *
 * @param token Token representing a program point.
 * @return Parsed ProgramPoint structure.
 *
 * @complexity O(n), where n is the token length.
 */
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

/**
 * @brief Reads and parses the live ranges file.
 *
 * Each line represents one variable and its corresponding program points.
 *
 * @param filename Name of the live ranges file.
 * @return Vector containing all parsed live ranges.
 *
 * @complexity O(l * p), where l is the number of lines and p is the number of program points.
 */
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

/**
 * @brief Reads and parses the register settings file.
 *
 * Reads the number of registers and the selected allocation algorithm.
 *
 * @param filename Name of the settings file.
 * @return Parsed register settings.
 *
 * @complexity O(l), where l is the number of lines in the file.
 */
RegisterSettings parseRegistersFile(string filename) {
    RegisterSettings settings;
    settings.registers = 0;
    settings.algorithm = "";
    settings.param = 0;
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

            size_t commaPos = value.find(',');
            string algoName;
            int algoParam = 0;
            if (commaPos == string::npos) {
                algoName = trim(value);
            }
            else {
                algoName = trim(value.substr(0, commaPos));
                string paramStr = trim(value.substr(commaPos + 1));
                algoParam = stoi(paramStr);
            }
            if (algoName == "basic" || algoName == "splitting" || algoName == "spilling" || algoName == "free") {
                settings.algorithm = algoName;
                settings.param = algoParam;
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

/**
 * @brief Parses all input files used by the program.
 *
 * Reads live ranges, register settings and output configuration.
 *
 * @param rangesFile Live ranges input file.
 * @param registersFile Register settings input file.
 * @param outputFile Output filename.
 * @return Structure containing all parsed input data.
 *
 * @complexity O(l * p).
 */
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
        cerr << "Errors while reading input files:" << endl;
        for (string error : errors) {
            cerr << "- " << error << endl;
        }
    }
    return data;
}