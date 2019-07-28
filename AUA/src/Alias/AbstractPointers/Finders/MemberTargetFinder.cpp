#include <utility>
#include <llvm/IR/Instructions.h>

//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h"

MemberTargetFinder::MemberTargetFinder(const CompositeFinder *compositeFinder, const int memberIndex,
                                       const llvm::GetElementPtrInst *gepInst) : compositeFinder(
        compositeFinder), memberIndex(memberIndex), gepInst(gepInst) {}

std::set<AbstractTarget> MemberTargetFinder::findTargets(Configuration *conf) const {

    CompositeSetValue *composites = compositeFinder->findComposites(conf);

    std::set<AbstractTarget> result;

    for (auto comp : composites->asSet()) {

        result.insert(comp->getMemberTarget(memberIndex));
    }


    return result;

}

std::list<const llvm::Instruction *> MemberTargetFinder::getAssociatedInsts() const {

    auto result = compositeFinder->getAssociatedInsts();
    auto inst = llvm::cast<llvm::Instruction>(gepInst);
    result.push_back(inst);

    return result;

}
