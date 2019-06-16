//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/VarAllocationOp.h"

VarAllocationOp::VarAllocationOp(std::string name, int size, const llvm::AllocaInst *allocaInst)
        : name(name), size(size), allocaInst(allocaInst) {}

Configuration* VarAllocationOp::apply(Configuration* in) {

    VarRef* var = new VarRef(name, 0, size);

    in->vars.insert_or_assign(name, var);

    return in;
}

std::vector<llvm::Instruction *> VarAllocationOp::getAssocInstructions() {
    std::vector<llvm::Instruction *> result;
    result.push_back((llvm::Instruction*) allocaInst);

    return result;
}
