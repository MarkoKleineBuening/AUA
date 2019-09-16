//
// Created by mlaupichler on 15.09.19.
//

#include "AUA/Alias/AbstractPointers/Finders/NullTargetFinder.h"

std::set<AbstractTarget> NullTargetFinder::findTargets(Configuration *conf) const {
    return std::set<AbstractTarget>();
}

std::list<const llvm::Instruction *> NullTargetFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}
