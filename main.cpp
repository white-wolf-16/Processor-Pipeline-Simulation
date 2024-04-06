#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm> 

using namespace std;

class Instruction {
public:
    string pc;
    int instructionType;
    vector<string> dependencies;

    Instruction(const string& pc, int instructionType, const vector<string>& dependencies) 
        : pc(pc), instructionType(instructionType), dependencies(dependencies) {}
};

vector<Instruction> parseInputTrace(const string& traceFile, unsigned int startInst, unsigned int instCount) {
    vector<Instruction> instructions;
    string line;
    unsigned int lineCount = 0;
    ifstream file(traceFile);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return instructions;
    }
    while (getline(file, line)) {
        lineCount++;
        // Skip lines until startInst is reached
        if (lineCount < startInst) {
            continue;
        }
        istringstream iss(line);
        string token;
        vector<string> dependencies;
        int count = 0;
        string pc;
        int instructionType;
        while (getline(iss, token, ',')) {
            if (count == 0) {
                pc = token;
            } else if (count == 1) {
                instructionType = stoi(token);
            } else {
                dependencies.push_back(token);
            }
            count++;
        }
        instructions.push_back(Instruction(pc, instructionType, dependencies));
        // Break if instCount instructions have been read
        if (instructions.size() >= instCount) {
            break; 
        }
    }
    return instructions;
}


void printParsedInput(const vector<Instruction>& instructions, int numLines) {
    cout << "[Debug]: Print Instruction Vector" << endl;
    for (const auto& ins : instructions) {
        cout << "PC: " << ins.pc << ", Type: " << ins.instructionType << ", Dependencies: ";
        for (const auto& dep : ins.dependencies) {
            cout << dep << " ";
        }
        cout << endl;
    }
    // TODO print until numLines only
}


int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " <trace_file> <start_inst> <inst_count> <W>" << endl;
        return 1;
    }

    string traceFile = argv[1];
    int startInst = stoi(argv[2]);
    int instCount = stoi(argv[3]);
    int W = stoi(argv[4]);

    // Handle input errors
    // if ...

    // Parse Input
    vector<Instruction> instructions = parseInputTrace(traceFile, startInst, instCount);

    // Debug print
    printParsedInput(instructions, 10);

    // Simulation...

    return 0;
}
