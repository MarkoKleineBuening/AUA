//
// Created by mlaupichler on 18.07.19.
//

#include <llvm/IR/Instructions.h>
#include "AUA/Alias/AbstractPointers/Finders/FromPointerTargetFinder.h"

std::set<AbstractTarget> FromPointerTargetFinder::findTargets(Configuration *conf) const {
    return pointerFinder->findPointers(conf)->getMergedTargets();
}

FromPointerTargetFinder::FromPointerTargetFinder(const PointerFinder *pointerFinder, const llvm::LoadInst *loadInst)
        : pointerFinder(pointerFinder), loadInst(loadInst) {}

std::list<const llvm::Instruction *> FromPointerTargetFinder::getAssociatedInsts() const {

    auto result = pointerFinder->getAssociatedInsts();
    result.push_back(loadInst);

    return result;
}
