//
// Created by mlaupichler on 07.08.19.
//

#include "AUA/Alias/AbstractPointers/Finders/CallPointerFinder.h"

std::list<const llvm::Instruction *> CallPointerFinder::getAssociatedInsts() const {

    auto result = std::list<const llvm::Instruction*>();
    result.push_back(callInst);
    return result;
}

CallPointerFinder::CallPointerFinder(const PointerFormat &expectedFormat, const llvm::CallInst *callInst)
        : PointerFinder(expectedFormat), callInst(callInst) {}
