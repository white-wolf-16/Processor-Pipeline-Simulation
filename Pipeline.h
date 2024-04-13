#ifndef PIPELINE_H
#define PIPELINE_H

#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Instruction.h"

using namespace std;

class Pipeline {
private:
    ifstream file;
    string line;

    unsigned int traceNumber;

    unsigned int width;
    unsigned int clock;
    unsigned int size; // Number of instruction in pipeline

    unsigned int endInst;

    queue<Instruction*> IF;
    queue<Instruction*> ID;
    queue<Instruction*> EX;
    queue<Instruction*> MEM;
    queue<Instruction*> WB;

    unordered_map<string, Instruction*> instructionMap;

    bool Stall;
    bool ALU;
    bool FP;
    bool Read;
    bool Write;

    int totalRetired;
    int totalBranch;
    int totalALU;
    int totalFP;
    int totalRead;
    int totalWrite;

public:
    Pipeline(unsigned int width, string traceFile);
    void simulatePipeline(unsigned int startInst, unsigned int instCount);

private:
    Instruction* getNextInstruction();
    bool dependenciesSatisfied(Instruction& Ins);

    void retireInstruction();// WB list
    void moveInstructionToWB();// MEM list
    void moveInstructionToMEM();// EX list
    void moveInstructionToEX();// ID list
    void moveInstructionToID();// IF list
    void moveNextInstructionToIF();// Load traces for current cycle
};

#endif // PIPELINE_H
