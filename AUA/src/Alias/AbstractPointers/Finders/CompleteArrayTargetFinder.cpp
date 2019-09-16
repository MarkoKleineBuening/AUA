//
// Created by mlaupichler on 15.09.19.
//

#include "AUA/Alias/AbstractPointers/Finders/CompleteArrayTargetFinder.h"

std::set<AbstractTarget> CompleteArrayTargetFinder::findTargets(Configuration *conf) const {

    auto targets = std::set<AbstractTarget>();
    auto arrays = arrayFinder->findComposites(conf);
    auto format = arrays->getFormat();
    int n = format.memberCount;
    for (auto a : arrays->asSet()) {
        for(int i = 0; i < n; ++i) {
            targets.insert(a->getMemberTarget(i));
        }
    }

    return targets;
}

std::list<const llvm::Instruction *> CompleteArrayTargetFinder::getAssociatedInsts() const {

    auto assocInsts = arrayFinder->getAssociatedInsts();
    auto inst = llvm::cast<llvm::Instruction>(gepInst);
    assocInsts.push_back(inst);
}

CompleteArrayTargetFinder::CompleteArrayTargetFinder(const CompositeFinder *arrayFinder,
                                                     const llvm::GetElementPtrInst *gepInst) : arrayFinder(arrayFinder),
                                                                                               gepInst(gepInst) {}
