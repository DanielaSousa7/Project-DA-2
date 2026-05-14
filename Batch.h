/**
* @file Batch.h
 * @brief Interface for batch-mode execution.
 *
 * Provides a function to run the register allocation tool in non-interactive
 * batch mode, reading input files and writing the output to a specified file.
 */

#ifndef BATCH_H
#define BATCH_H

#include <string>
using namespace std;

/**
 * @brief Runs the allocation tool in batch mode.
 *
 * Parses the live ranges file and registers file, performs register allocation
 * according to the algorithm specified in the registers file, and writes the
 * result to the given output file. Errors are printed to standard error.
 *
 * @param rangesFile Path to the file containing variable live ranges.
 * @param registersFile Path to the file containing register count and algorithm.
 * @param outputFile Path where the allocation result will be written.
 *
 * @complexity O(F + A) where F is the size of input files (parsing)
 *             and A is the complexity of the selected allocation algorithm.
 *             Parsing is linear in the number of lines.
 */
void batchMode(string rangesFile, string registersFile, string outputFile);

#endif