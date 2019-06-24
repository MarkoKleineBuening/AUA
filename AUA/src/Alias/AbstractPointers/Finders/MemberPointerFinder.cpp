//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/MemberPointerFinder.h"

AbstractPointer * MemberPointerFinder::findPointer(Configuration *configuration) const {

    assert(memberIndices.size() > 0);

    CompositeRef* comp = configuration->composites[compositeName];

    for (auto LI = memberIndices.begin(), LE = --memberIndices.end(); LI != LE; ++LI) {

        comp = comp->getCompositeMember(*LI);
        llvm::outs() << *LI << ", ";
    }

    llvm::outs() << memberIndices.back() << "\n";

    return comp->getPointerMember(memberIndices.back());

}

MemberPointerFinder::MemberPointerFinder(const std::string compositeName, const std::list<int> memberIndices)
        : compositeName(compositeName), memberIndices(memberIndices) {}
