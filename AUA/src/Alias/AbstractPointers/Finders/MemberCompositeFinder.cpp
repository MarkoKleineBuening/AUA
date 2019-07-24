#include <utility>

#include <utility>

//
// Created by mlaupichler on 13.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberCompositeFinder.h"

CompositeSetValue *MemberCompositeFinder::findComposites(Configuration *configuration) const {

    assert(memberIndices.size() > 0);

    CompositeSetValue *baseComposites = baseCompositeFinder->findComposites(configuration);

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

MemberCompositeFinder::MemberCompositeFinder(CompositeFinder *topLevelCompositeFinder,
                                             CompositeFormat expectedFormat, std::list<int> memberIndices)
        : CompositeFinder(expectedFormat), baseCompositeFinder(topLevelCompositeFinder),
          memberIndices(std::move(memberIndices)) {}

