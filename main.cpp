#include "Pipeline.h"

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
    // if ...

    Pipeline* pipeline = new Pipeline(W, traceFile);

    // Simulation...
    pipeline->simulatePipeline(startInst, instCount);

    return 0;
}
