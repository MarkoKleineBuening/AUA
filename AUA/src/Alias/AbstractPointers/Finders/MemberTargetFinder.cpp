//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h"

MemberTargetFinder::MemberTargetFinder(const std::string &compositeName, const std::list<int> memberIndices) : compositeName(
        compositeName), memberIndices(memberIndices) {}

AbstractTarget MemberTargetFinder::findTarget(Configuration *conf) const {

    assert(memberIndices.size() > 0);

    CompositeRef* comp = conf->composites[compositeName];

    for (auto LI = memberIndices.begin(), LE = memberIndices.end(); LI != --LE; ++LI) {

        comp = comp->getCompositeMember(*LI);

    }


    return comp->getMemberTarget(memberIndices.back());

}
