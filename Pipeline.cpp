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

        // reopening locked ALU and FP for next cycle
        ALU = 0;
        FP = 0;
    }
}

Instruction Pipeline::getNextInstruction() {
    // Implement logic to get the next instruction
}

bool Pipeline::dependenciesSatisfied(Instruction& Ins) {
    // Implement logic to check if dependencies are satisfied
    for (Instruction* dependency : Ins.dependencies) {
        if (!dependency->executed) {
            return false;
        }
    }
    return true;
}

void Pipeline::retireInstruction() {
    // Implement logic to retire instructions
}

void Pipeline::moveInstructionToWB() {
    // Implement logic to move instructions to WB
}

void Pipeline::moveInstructionToMEM() {
    // Implement logic to move instructions to MEM
    while (!EX.empty() && (MEM.size() < width)) {
        Instruction* instr = EX.front();
        if (dependenciesSatisfied(instr)) {
            if (instr->instructionType == INTEGER_INSTRUCTION) {
                if (ALU == 1)   // if ALU is already locked, break
                    return;
                else
                    ALU = 1;    // otherwise lock ALU - *should reset again for the next cycle in sim
            }
            if (instr->instructionType == FLOATING_POINT_INSTRUCTION) {
                if (FP == 1)   // if FP is already locked, break
                    return;
                else
                    FP = 1;    // otherwise lock FP - *should reset again for the next cycle in sim
            }
            if (instr->instructionType == BRANCH) {
                Stall = 0;     // continue instruction
            }
            EX.pop();

            if (instr->instructionType == INTEGER_INSTRUCTION || instr->instructionType == FLOATING_POINT_INSTRUCTION) {
                instr->executed = true;
            }
            MEM.push(instr);

        }

    }
}

void Pipeline::moveInstructionToEX() {
    // Implement logic to move instructions to EX
    while (!ID.empty() && (EX.size() < width)) {
        Instruction* instr = ID.front();
        ID.pop();
        EX.push(instr);
    }
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
