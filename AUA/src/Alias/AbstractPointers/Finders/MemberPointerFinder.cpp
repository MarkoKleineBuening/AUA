//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberPointerFinder.h"

PointerSetValue *MemberPointerFinder::findPointers(Configuration *configuration) const {

    CompositeSetValue *composites = compositeFinder->findComposites(configuration);

    PointerSetValue *resultPointers = new PointerSetValue(expectedFormat);

    for (AbstractComposite *comp : composites->asSet()) {

        AbstractPointer *pointer = comp->getPointerMember(memberIndex);
        resultPointers->include(pointer);

    }


    return resultPointers;

}

MemberPointerFinder::MemberPointerFinder(const CompositeFinder *compositeFinder, const int memberIndex,
                                         const PointerFormat &expectedFormat,
                                         const llvm::GetElementPtrInst *gepInst)
        : PointerFinder(expectedFormat), compositeFinder(compositeFinder), memberIndex(memberIndex), gepInst(gepInst) {}

std::list<const llvm::Instruction *> MemberPointerFinder::getAssociatedInsts() const {

    auto result = compositeFinder->getAssociatedInsts();
    auto inst = llvm::cast<llvm::Instruction>(gepInst);
    result.push_back(inst);

    return result;

}
