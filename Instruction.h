#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <string>

using namespace std;

enum InstructionType {
    INTEGER_INSTRUCTION = 1,
    FLOATING_POINT_INSTRUCTION = 2,
    BRANCH = 3,
    LOAD = 4,
    STORE = 5
};

InstructionType getType(int value);

class Instruction {
public:
    string pc;
    InstructionType instructionType;
    vector<Instruction*> dependencies;
    bool executed;// To flag if it has been satisfied as a dependency

    Instruction();
};

#endif // INSTRUCTION_H
