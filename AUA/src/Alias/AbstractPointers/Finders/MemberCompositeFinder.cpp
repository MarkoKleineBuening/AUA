#include <utility>

#include <utility>

#include <utility>

//
// Created by mlaupichler on 13.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberCompositeFinder.h"

CompositeSetValue *MemberCompositeFinder::findComposites(Configuration *configuration) const {

    assert(!memberIndices.empty());

    CompositeSetValue *baseComposites = topLevelCompositeFinder->findComposites(configuration);

    std::list<int> compMemberIndices = std::list<int>(memberIndices);
    int compositeMemberIndex = compMemberIndices.back();
    compMemberIndices.pop_back();

    CompositeSetValue *result = new CompositeSetValue(expectedFormat);

    for (auto superComposite : baseComposites->asSet()) {

        superComposite = superComposite->getSubCompositeRecursively(compMemberIndices);
        result->include(superComposite->getCompositeMember(compositeMemberIndex));

    }

    return result;

}

MemberCompositeFinder::MemberCompositeFinder(const CompositeFinder *topLevelCompositeFinder,
                                             const CompositeFormat &expectedFormat, const std::list<int>& memberIndices,
                                             const std::list<llvm::GetElementPtrInst *>& gepInsts)
        : CompositeFinder(expectedFormat), topLevelCompositeFinder(topLevelCompositeFinder),
          memberIndices(memberIndices), gepInsts(gepInsts) {}

std::list<const llvm::Instruction *> MemberCompositeFinder::getAssociatedInsts() const {

    auto result = topLevelCompositeFinder->getAssociatedInsts();
    result.insert(result.end(), gepInsts.begin(), gepInsts.end());

    return result;

}

