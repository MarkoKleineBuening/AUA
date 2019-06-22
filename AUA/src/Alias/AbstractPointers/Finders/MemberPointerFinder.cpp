//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberPointerFinder.h"

AbstractPointer * MemberPointerFinder::findPointer(Configuration *configuration) const {

    assert(memberIdx >= 0);

    CompositeRef* comp = configuration->composites[compositeName];

    return comp->getPointerMember(memberIdx);

}

MemberPointerFinder::MemberPointerFinder(const std::string compositeName, const int memberIdx)
        : compositeName(compositeName), memberIdx(memberIdx) {}
