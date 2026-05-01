#include "Batch.h"
#include "Parser.h"
#include <iostream>

using namespace std;

void batchMode(string rangesFile, string registersFile, string outputFile) {
    InputData data = parseInputFiles(rangesFile, registersFile, outputFile);
    if (data.hasErrors) {
        cerr << "Errors found while reading input files. Aborting batch mode." << endl;
        return;
    }
    cout << "Loaded: " << data.liveRanges.size() << " live range(s), "
         << data.settings.registers << " register(s), algorithm: "
         << data.settings.algorithm << endl;

    /*
        Aqui depois vais chamar as próximas fases do projeto, por exemplo:

        1. Criar webs
        2. Criar grafo de interferência
        3. Fazer alocação de registos
        4. Escrever o ficheiro de output

        Exemplo futuro:

        vector<Web> webs = buildWebs(data.liveRanges);
        Graph graph = buildInterferenceGraph(webs);
        AllocationResult result = allocateRegisters(graph, data.settings);
        writeOutput(result, data.outputFile);
    */

    cout << "[Output written to: " << outputFile << "]\n";
}