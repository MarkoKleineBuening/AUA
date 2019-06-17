//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/PointerAllocationOp.h"

PointerAllocationOp::PointerAllocationOp(std::string n, int l, const llvm::AllocaInst *allocInst)
        : name(n), level(l), allocInst(allocInst) {}


Configuration* PointerAllocationOp::apply(Configuration* in) {

    AbstractPointer* pointer = new AbstractPointer(name, 0, level);
    pointer->setOnlyAssocInst((llvm::Instruction*) allocInst);

    in->pointers.insert_or_assign(name, pointer);

    return in;

}

std::set<llvm::Instruction *> PointerAllocationOp::getAssocInstructions() {
    std::set<llvm::Instruction *> result;
    result.insert((llvm::Instruction*) allocInst);

    return result;
}
