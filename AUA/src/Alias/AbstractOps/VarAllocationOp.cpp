//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/VarAllocationOp.h"

VarAllocationOp::VarAllocationOp(std::string name, int size, const llvm::AllocaInst *allocaInst)
        : name(name), size(size), allocaInst(allocaInst) {}

Configuration *VarAllocationOp::apply(Configuration *in) {

    AbstractVar *var = new AbstractVar(name, size);

    in->vars.insert_or_assign(name, var);

    return in;
}

std::set<llvm::Instruction *> VarAllocationOp::getAssocInstructions() {
    std::set<llvm::Instruction *> result;
    result.insert((llvm::Instruction *) allocaInst);

    return result;
}
