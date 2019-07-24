#include <utility>

//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h"

MemberTargetFinder::MemberTargetFinder(CompositeFinder *compositeFinder, int memberIndex) : compositeFinder(
        compositeFinder), memberIndex(memberIndex) {}

std::set<AbstractTarget> MemberTargetFinder::findTargets(Configuration *conf) const {

    CompositeSetValue *composites = compositeFinder->findComposites(conf);

    std::set<AbstractTarget> result;

    for (auto comp : composites->asSet()) {

        result.insert(comp->getMemberTarget(memberIndex));
    }


    return result;

}
