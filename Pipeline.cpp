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
        Read = 0;
        Write = 0;

        totalRetired = 0;
        totalBranch = 0;
        totalALU = 0;
        totalFP = 0;
        totalRead = 0;
        totalWrite = 0;
    }
}

Instruction* Pipeline::getNextInstruction(const string& traceFile, unsigned int getLine) {
    // Implement logic to get the next instruction
    string line;
    unsigned int lineCount = 0;
    ifstream file(traceFile);

    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return nullptr;
    }

    // Skip lines until startInst is reached
    while (getline(file, line)) {
        lineCount++;
        if (lineCount < getLine) {
            continue;
        }
        break;
    }

    istringstream iss(line);
    string token;
    vector<string> tokens;
    while (getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    if (tokens.size() < 2) {
        cout << "Error: Malformed instruction on line " << lineCount << "." << endl;
        return nullptr;
    }

    // Create the instruction object
    Instruction* inst = new Instruction();
    inst->pc = tokens[0];
    inst->instructionType = getType(stoi(tokens[1]));

    // Handle dependencies if present
    for (size_t i = 2; i < tokens.size(); ++i) {
        if (instructionMap.find(tokens[i]) != instructionMap.end()) {
            Instruction* dependency = instructionMap[tokens[i]];
            inst->dependencies.push_back(dependency);
        }
        else {
            cout << "[Debug] Dependency '" << tokens[i] << "' not found." << endl;
        }
    }

    // Store the instruction in the instruction map
    instructionMap[inst->pc] = inst;

    return inst;
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
    while (!WB.empty() && (WB.size() < width)) {
        Instruction* instr = MEM.front();
        if(instr->instructionType == BRANCH){
            totalRetired+=1;
            totalBranch+=1;
        }   
        if(instr->instructionType == INTEGER_INSTRUCTION){
            totalRetired+=1;
            totalALU+=1;
        }   
        if(instr->instructionType == FLOATING_POINT_INSTRUCTION){
            totalRetired+=1;
            totalFP+=1;
        }   
        if(instr->instructionType == LOAD){
            totalRetired+=1;
            totalRead+=1;
        }  
        if(instr->instructionType == STORE){
            totalRetired+=1;
            totalWrite+=1;
        }  
        WB.pop();

        //call function to generate histogram for retired instructions

    }

    
}

void Pipeline::moveInstructionToWB() {
    // Implement logic to move instructions to WB
    while (!MEM.empty() && (MEM.size() < width)) {
        Instruction* instr = MEM.front();
        if (dependenciesSatisfied(*instr)) {
            if(instr->instructionType == BRANCH){
                moveNextInstructionToIF(); // branch has executed so now a new instruction can be fetched
            }   
            if (instr->instructionType == LOAD) {
                if (Read == 1)   // read in use by an instruction
                    return;
                else
                    Read = 1;    
            }
            if (instr->instructionType == STORE) {
                if (Write == 1)   
                    return;
                else
                    Write = 1;    
            }
            MEM.pop();
            if (instr->instructionType == LOAD || instr->instructionType == STORE) {
            instr->executed = true;
            }
            WB.push(instr); 
        }
    }
    
}

void Pipeline::moveInstructionToMEM() {
    // Implement logic to move instructions to MEM
    while (!EX.empty() && (MEM.size() < width)) {
        Instruction* instr = EX.front();
        if (dependenciesSatisfied(*instr)) {
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
