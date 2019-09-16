//
// Created by mlaupichler on 15.09.19.
//

#include "AUA/Alias/AbstractPointers/Finders/CompleteArrayCompositeFinder.h"

CompositeSetValue *CompleteArrayCompositeFinder::findComposites(Configuration *configuration) const {

    auto composites = new CompositeSetValue(expectedFormat);
    auto arrays = parentArrayFinder->findComposites(configuration);
    auto format = arrays->getFormat();
    int n = format.memberCount;
    for (auto a : arrays->asSet()) {
        for(int i = 0; i < n; ++i) {
            composites->include(a->getCompositeMember(i));
        }
    }

    return composites;

}

std::list<const llvm::Instruction *> CompleteArrayCompositeFinder::getAssociatedInsts() const {

    auto assocInsts = parentArrayFinder->getAssociatedInsts();
    auto inst = llvm::cast<llvm::Instruction>(gepInst);
    assocInsts.push_back(inst);

}

CompleteArrayCompositeFinder::CompleteArrayCompositeFinder(CompositeFinder *parentArrayFinder,
                                                           const CompositeFormat &expectedFormat,
                                                           llvm::GetElementPtrInst *gepInst) : CompositeFinder(
        expectedFormat), parentArrayFinder(parentArrayFinder), gepInst(gepInst) {}
