#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/PointerAllocationOp.h"

PointerAllocationOp::PointerAllocationOp(std::string name, PointerFormat ptrFormat, const llvm::AllocaInst *allocInst)
        : name(std::move(name)), pointerFormat(ptrFormat), allocaInst(allocInst) {}


Configuration *PointerAllocationOp::apply(Configuration *in) {

    AbstractPointer *pointer = new AbstractPointer(name, pointerFormat);
    pointer->setOnlyAssocInst((llvm::Instruction *) allocaInst);

    in->pointers.insert_or_assign(name, pointer);

    return in;

}
