//
// Created by mlaupichler on 15.09.19.
//

#include "AUA/Alias/AbstractPointers/Finders/CompleteArrayPointerFinder.h"

PointerSetValue *CompleteArrayPointerFinder::findPointers(Configuration *configuration) const {

    auto pointers = new PointerSetValue(expectedFormat);
    auto arrays = arrayFinder->findComposites(configuration);
    auto format = arrays->getFormat();
    int n = format.memberCount;
    for (auto a : arrays->asSet()) {
        for(int i = 0; i < n; ++i) {
            pointers->include(a->getPointerMember(i));
        }
    }

    return pointers;
}

std::list<const llvm::Instruction *> CompleteArrayPointerFinder::getAssociatedInsts() const {

    auto assocInsts = arrayFinder->getAssociatedInsts();
    auto inst = llvm::cast<llvm::Instruction>(gepInst);
    assocInsts.push_back(inst);
}

CompleteArrayPointerFinder::CompleteArrayPointerFinder(const CompositeFinder *arrayFinder,
                                                       const PointerFormat &expectedFormat,
                                                       const llvm::GetElementPtrInst *gepInst) : PointerFinder(
        expectedFormat), arrayFinder(arrayFinder), gepInst(gepInst) {}
