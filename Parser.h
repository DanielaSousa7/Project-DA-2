/**
* @file Parser.h
 * @brief Functions for parsing input files and program data.
 */

#ifndef PARSER_H
#define PARSER_H

#include "Structs.h"
#include <string>

using namespace std;

/**
 * @brief Removes leading and trailing whitespace from a string.
 *
 * @param s Input string.
 * @return Trimmed string.
 *
 * @complexity O(n), where n is the string length.
 */
string trim(string s);

/**
 * @brief Parses a program point token.
 *
 * Converts a token into a ProgramPoint structure.
 *
 * @param token String token representing a program point.
 * @return Parsed ProgramPoint.
 *
 * @complexity O(n), where n is the token length.
 */
ProgramPoint parseProgramPoint(string token);

/**
 * @brief Reads and parses the live ranges file.
 *
 * @param filename Name of the live ranges input file.
 * @return Vector containing all parsed live ranges.
 *
 * @complexity O(l * p), where l is the number of lines and p is the number of program points.
 */
vector<LiveRange> parseLiveRangesFile(string filename);

/**
 * @brief Reads and parses the register settings file.
 *
 * @param filename Name of the registers input file.
 * @return Parsed register settings.
 *
 * @complexity O(l), where l is the number of lines in the file.
 */
RegisterSettings parseRegistersFile(string filename);

/**
 * @brief Parses all input files required by the program.
 *
 * @param rangesFile Live ranges input file.
 * @param registersFile Register settings input file.
 * @param outputFile Output filename.
 * @return Structure containing all parsed input data.
 *
 * @complexity O(l * p).
 */
InputData parseInputFiles(string rangesFile, string registersFile, string outputFile);

#endif