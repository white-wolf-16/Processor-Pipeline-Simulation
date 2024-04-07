#ifndef PIPELINE_H
#define PIPELINE_H

#include <queue>
#include <string>
#include "./main.cpp"// Import instruction definition

using namespace std;

class Pipeline {
private:
    int width;
    int clock;

    queue<Instruction*> IF;
    queue<Instruction*> ID;
    queue<Instruction*> EX;
    queue<Instruction*> MEM;
    queue<Instruction*> WB;

    int Stall;
    int ALU;
    int FP;
    int Read;
    int Write;

    queue<Instruction*> instructionList;

public:
    Pipeline(int W);
    void simulatePipeline();

private:
    Instruction getNextInstruction();
    bool dependenciesSatisfied(Instruction& Ins);

    void retireInstruction();// WB list
    void moveInstructionToWB();// MEM list
    void moveInstructionToMEM();// EX list
    void moveInstructionToEX();// ID list
    void moveInstructionToID();// IF list
    void moveNextInstructionToIF();// Load traces for current cycle
};

#endif // PIPELINE_H
