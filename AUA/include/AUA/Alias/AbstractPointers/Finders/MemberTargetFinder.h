//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_MEMBERTARGETFINDER_H
#define AUA_MEMBERTARGETFINDER_H


#include "TargetFinder.h"
#include "CompositeFinder.h"

class MemberTargetFinder : public TargetFinder {

private:

    const CompositeFinder *compositeFinder;
    const int memberIndex;

public:

    /**
     * Creates a MemberTargetFinder holding the given information to find a target that is a member of a composite later.
     * @param compositeFinder the composite of which the target is a member.
     * @param memberIndex a list of indices, one for each level of recursive composites starting at the root composite (the one with the given name).
     */
    MemberTargetFinder(CompositeFinder *compositeFinder, int memberIndex);

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

};


#endif //AUA_MEMBERTARGETFINDER_H
