//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/PointerAllocationOp.h"

PointerAllocationOp::PointerAllocationOp(std::string n, int l, const llvm::AllocaInst *allocInst)
        : name(n), level(l), allocInst(allocInst) {}


Configuration* PointerAllocationOp::apply(Configuration* in) {

    AbstractPointer* pointer = new AbstractPointer(name, 0, level);

    in->pointers.insert_or_assign(name, pointer);

    return in;

}

std::vector<llvm::Instruction *> PointerAllocationOp::getAssocInstructions() {
    std::vector<llvm::Instruction *> result;
    result.push_back((llvm::Instruction*) allocInst);

    return result;
}
