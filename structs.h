#ifndef STRUCTS_H
#define STRUCTS_H

/**
 * @file structs.h
 * @brief Main data structures used in the register allocation project.
 */

#include <string>
#include <vector>
#include "data_structures/Graph.h"

using namespace std;

/**
 * @brief Represents one program point in a live range.
 */
struct ProgramPoint {
    int code_line;          /**< Program line number. */
    bool isDefinition;      /**< True if the point is a definition. */
    bool isLastUse;         /**< True if the point is the last use. */
};

/**
 * @brief Represents one live range of a variable.
 */
struct LiveRange {
    string variable;              /**< Variable name. */
    vector<ProgramPoint> points;  /**< Program points where the variable is live. */
};

/**
 * @brief Stores the register allocation settings.
 */
struct RegisterSettings {
    int registers;       /**< Maximum number of available registers. */
    string algorithm;    /**< Algorithm name: basic, spilling, splitting or free. */
    int param;           /**< Optional parameter used by spilling or splitting. */
};

/**
 * @brief Stores all input data.
 */
struct InputData {
    vector<LiveRange> liveRanges; /**< Parsed live ranges. */
    RegisterSettings settings;    /**< Parsed register settings. */
    string outputFile;            /**< Output file path. */
    bool hasErrors = false;       /**< True if input parsing found errors. */
};

/**
 * @brief Represents a live web.
 */
struct Web {
    string variable;              /**< Variable associated with the web. */
    int start;                    /**< First program line of the web. */
    int end;                      /**< Last program line of the web. */
    int id;                       /**< Web identifier. */
    vector<ProgramPoint> points;  /**< Program points included in the web. */
};

/**
 * @brief Represents the interference graph between webs.
 */
struct InterferenceGraph {
    Graph<int> graph;             /**< Graph structure from the TP classes. */
    vector<Web> webs;             /**< Webs represented as graph vertices. */
    vector<vector<bool>> adj;     /**< Adjacency matrix for quick interference checks. */
};

/**
 * @brief Stores the result of a register allocation algorithm.
 */
struct AllocationResult {
    int usedRegisters;                         /**< Number of registers used. */
    vector<int> webToRegister;                 /**< Register assigned to each web, or -1 for memory. */
    bool success;                              /**< True if allocation was successful. */
    vector<Web> webs;                          /**< Final webs after allocation. */
    string algorithm_used = "basic";           /**< Algorithm used to produce the result. */
    vector<pair<int, vector<int>>> splitInfo;  /**< Information about split webs. */
};

#endif