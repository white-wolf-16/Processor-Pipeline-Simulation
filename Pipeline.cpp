#include "Pipeline.h"

Pipeline::Pipeline(unsigned int width, string traceFile)
    : 
      traceNumber(0),  // Initialize traceNumber
      width(width),  // Initialize width 
      clock(0),  // Initialize clock
      size(0), // Initialize number of instructions
      endInst(0), // Initialize ending instruction
      Stall(false), ALU(false), FP(false), Read(false), Write(false) // Initialize resource flags
{
    // Initialize queues and maps here if needed
    file.open(traceFile);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
    }
    for (int i = 0; i < 6; i++) {
        totals[i] = 0;
    }
}

void Pipeline::simulatePipeline(unsigned int startInst, unsigned int instCount) {
    // Go to starting instruction in file
    for (unsigned int i = 1; i < startInst; i++) {
        getline(file, line);
    }

    traceNumber = startInst;
    endInst = traceNumber + instCount - 1;
    while (traceNumber <= endInst || size > 0) {
        // FOR DEBUGGING

        // if (clock % 1000000 == 0) {
        //     cout << traceNumber << endl;
        //     cout << instructionMap.size() << endl;
        //     cout << "Trace: " << traceNumber << endl;
        //     cout << "Current size of IF queue: " << IF.size() << endl;
        //     cout << "Current size of ID queue: " << ID.size() << endl;
        //     cout << "Current size of EX queue: " << EX.size() << endl;
        //     cout << "Current size of MEM queue: " << MEM.size() << endl;
        //     cout << "Current size of WB queue: " << WB.size() << endl;
        //     cout << endl;
        // }

        // cout << "Trace: " << traceNumber << endl;
        // cout << "Current size of IF queue: " << IF.size() << endl;
        // cout << "Current size of ID queue: " << ID.size() << endl;
        // cout << "Current size of EX queue: " << EX.size() << endl;
        // cout << "Current size of MEM queue: " << MEM.size() << endl;
        // cout << "Current size of WB queue: " << WB.size() << endl;
        // cout << endl;

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
    }

    cout << "Total Cycles: " << clock << endl << endl;
    cout << "Histogram: " << endl;
    cout << "Integer: " << static_cast<double>(totals[1]) * 100.0 /totals[0] << "%" << endl;
    cout << "Floating Point: " << static_cast<double>(totals[2]) * 100.0 /totals[0] << "%" << endl;
    cout << "Branched: " << static_cast<double>(totals[3]) * 100.0 /totals[0] << "%" << endl;
    cout << "Load: " << static_cast<double>(totals[4]) * 100.0 /totals[0] << "%" << endl;
    cout << "Store: " << static_cast<double>(totals[5]) * 100.0 /totals[0] << "%" << endl;
    cout << "Total: " << totals[0] << endl;
}

Instruction* Pipeline::getNextInstruction() {
    // Implement logic to get the next instruction
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return nullptr;
    }

    // Skip lines until startInst is reached
    vector<string> tokens;
    if (getline(file, line)) {
        istringstream iss(line);
        string token;

        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }
    }
    if (tokens.size() < 2) {
        cout << "Error: Malformed instruction." << endl;
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
    while (!WB.empty()) {
        Instruction* instr = WB.front();
        totals[static_cast<int>(instr->instructionType)]++;
        totals[0]++;
        size--;
        WB.pop();

        //call function to generate histogram for retired instructions

    }

    
}

void Pipeline::moveInstructionToWB() {
    // Implement logic to move instructions to WB
    while (!MEM.empty())
    {
        Instruction* instr = MEM.front();
        MEM.pop();
        WB.push(instr);
        if (instr->instructionType == LOAD || instr->instructionType == STORE) {
            instr->executed = true;
        }
    }
    
}

void Pipeline::moveInstructionToMEM() {
    // Implement logic to move instructions to MEM
    while (!EX.empty()) {
        Instruction* instr = EX.front();
        if(instr->instructionType == BRANCH){
            moveNextInstructionToIF(); // branch has executed so now a new instruction can be fetched
        }   
        if (instr->instructionType == LOAD) {
            if (Read)   // read in use by an instruction
                return;
            else
                Read = true;    
        }
        if (instr->instructionType == STORE) {
            if (Write)   
                return;
            else
                Write = true;    
        }
        EX.pop();
        if (instr->instructionType == INTEGER_INSTRUCTION || instr->instructionType == FLOATING_POINT_INSTRUCTION || instr->instructionType == BRANCH) {
            instr->executed = true;
        }
        MEM.push(instr);
    }
    
}

void Pipeline::moveInstructionToEX() {
    // Implement logic to move instructions to EX
    while (!ID.empty() && (EX.size() < width)) {
        Instruction* instr = ID.front();
        if (dependenciesSatisfied(*instr)) {
            if (instr->instructionType == INTEGER_INSTRUCTION) {
                if (ALU)   // if ALU is already locked, break
                    return;
                else
                    ALU = true;    // otherwise lock ALU - *should reset again for the next cycle in sim
            }
            if (instr->instructionType == FLOATING_POINT_INSTRUCTION) {
                if (FP)   // if FP is already locked, break
                    return;
                else
                    FP = true;    // otherwise lock FP - *should reset again for the next cycle in sim
            }
            if (instr->instructionType == BRANCH) {
                Stall = false;     // continue instruction
            }
            ID.pop();

            EX.push(instr);
        }
        else return;
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
    while (IF.size() < width && traceNumber <= endInst && !Stall) {
        Instruction* nextInstr = getNextInstruction();
        IF.push(nextInstr);
        traceNumber++;
        size++;
        if (nextInstr->instructionType == BRANCH) { // Branch
            Stall = true;
            break;
        }
    }
}
