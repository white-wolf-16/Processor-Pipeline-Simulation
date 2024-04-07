#include "pipeline.h"

Pipeline::Pipeline(int W) {
    width = W;
    // Initialize other variables here
}

void Pipeline::simulatePipeline() {
    while (true) {
        // Perform pipeline stages
        retireInstruction();
        moveInstructionToWB();
        moveInstructionToMEM();
        moveInstructionToEX();
        moveInstructionToID();
        moveNextInstructionToIF();

        clock++;
    }
}

Instruction Pipeline::getNextInstruction() {
    // Implement logic to get the next instruction
}

bool Pipeline::dependenciesSatisfied(Instruction& Ins) {
    // Implement logic to check if dependencies are satisfied
}

void Pipeline::retireInstruction() {
    // Implement logic to retire instructions
}

void Pipeline::moveInstructionToWB() {
    // Implement logic to move instructions to WB
}

void Pipeline::moveInstructionToMEM() {
    // Implement logic to move instructions to MEM
}

void Pipeline::moveInstructionToEX() {
    // Implement logic to move instructions to EX
}

void Pipeline::moveInstructionToID() {
    // Implement logic to move instructions to ID
    while (!IF.empty() && (ID.size() < width))
    {
        Instruction* instr = IF.front();
        IF.pop();
        ID.push(instr);
    }
}

void Pipeline::moveNextInstructionToIF() {
    // Implement logic to load traces for the current cycle
    if (Stall == 1) {
        while (IF.size() < width) {
            Instruction* nextInstr = instructionList.front();
            instructionList.pop();
            IF.push(nextInstr);
            if (nextInstr->instructionType == 3) { // Branch
                Stall == 1;
                break;
            }
        }
    }
}
