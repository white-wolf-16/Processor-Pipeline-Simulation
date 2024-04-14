#include "Pipeline.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " <trace_file> <start_inst> <inst_count> <W>" << endl;
        return 1;
    }

    string traceFile = argv[1];
    unsigned int startInst = stoi(argv[2]);
    unsigned int instCount = stoi(argv[3]);
    unsigned int W = stoi(argv[4]);

    // Handle input errors
    if (startInst < 1) {
        cerr << "Start instruction must be greater than 0, Try Again" << endl;
        return 1;
    }
    if (instCount < 1) {
        cerr << "Instruction count must be greater than 0,Try Again" << endl;
        return 1;
    }
    if (W < 1) {
        cerr << "Pipeline width must be greater than 0, Try Again" << endl;
        return 1;
    }

    Pipeline* pipeline = new Pipeline(W, traceFile);
    cout << "Simulating pipeline with starting instruction = " << startInst
        << "  pipeline width =   " << W
        << ", and number of instructions to simulate = " << instCount << endl;
    // Simulation...
    pipeline->simulatePipeline(startInst, instCount);

    // Clean up
    delete pipeline;

    return 0;
}