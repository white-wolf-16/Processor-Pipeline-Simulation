#include "Instruction.h"

InstructionType getType(int value) {
    return static_cast<InstructionType>(value);
}

Instruction::Instruction() {
    executed = false;
}
