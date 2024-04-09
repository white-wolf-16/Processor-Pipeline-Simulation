#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm> 
#include <unordered_map> 

using namespace std;

enum InstructionType {
    INTEGER_INSTRUCTION = 1,
    FLOATING_POINT_INSTRUCTION = 2,
    BRANCH = 3,
    LOAD = 4,
    STORE = 5
};

InstructionType getType(int value) {
    return static_cast<InstructionType>(value);
}

class Instruction {
public:
    string pc;
    InstructionType instructionType;
    vector<Instruction*> dependencies;
    bool executed;

    Instruction(const string& pc, InstructionType instructionType, const vector<Instruction*>& dependencies) 
        : pc(pc), instructionType(instructionType), dependencies(dependencies) {}

    Instruction() {
        executed = false;
    }
};

unordered_map<string, Instruction*> instructionMap;

// TODO: Bring parsing functionality into the Pipeline class and parse one at a time.
vector<Instruction*> parseInputTrace(const string& traceFile, unsigned int startInst, unsigned int instCount) {
    vector<Instruction*> instructions;
    string line;
    unsigned int lineCount = 0;
    ifstream file(traceFile);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return instructions;
    }
    while (getline(file, line)) {
        Instruction* inst = new Instruction();
        lineCount++;
        // Skip lines until startInst is reached
        if (lineCount < startInst) {
            continue;
        }
        istringstream iss(line);
        string token;
        int count = 0;
        string pc;
        int instructionType;
        while (getline(iss, token, ',')) {
            if (count == 0) {
                inst->pc = token;
                instructionMap[token] = inst;
            } else if (count == 1) {
                instructionType = stoi(token);
                inst->instructionType = getType(instructionType);
            } else {
                Instruction* dependency = instructionMap.at(token);
                inst->dependencies.push_back(dependency);
            }
            count++;
        }
        instructions.push_back(inst);
        // Break if instCount instructions have been read
        if (instructions.size() >= instCount) {
            break; 
        }
    }
    return instructions;
}


void printParsedInput(const vector<Instruction*>& instructions, int numLines) {
    cout << "[Debug]: Print Instruction Vector" << endl;
    for (const auto& ins : instructions) {
        cout << "PC: " << ins->pc << ", Type: " << ins->instructionType << ", Dependencies: ";
        for (const auto& dep : ins->dependencies) {
            cout << dep->pc << " ";
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
    vector<Instruction*> instructions = parseInputTrace(traceFile, startInst, instCount);

    // Debug print
    printParsedInput(instructions, 10);

    // Simulation...

    return 0;
}
