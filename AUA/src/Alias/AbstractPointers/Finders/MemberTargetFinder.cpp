//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h"

MemberTargetFinder::MemberTargetFinder(const std::string &compositeName, const int memberIdx) : compositeName(
        compositeName), memberIdx(memberIdx) {}

AbstractTarget MemberTargetFinder::findTarget(Configuration *conf) const {

    assert(memberIdx >= 0);

    CompositeRef* comp = conf->composites[compositeName];

    return comp->getMemberTarget(memberIdx);

}
