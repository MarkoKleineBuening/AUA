//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_MEMBERTARGETFINDER_H
#define AUA_MEMBERTARGETFINDER_H


#include "TargetFinder.h"

class MemberTargetFinder : public TargetFinder {

private:

    const std::string compositeName;
    const std::list<int> memberIndices;

public:

    /**
     * Creates a MemberTargetFinder holding the given information to find a target that is a member of a composite later.
     * @param compositeName the composite of which the target is a member.
     * @param memberIndices a list of indices, one for each level of recursive composites starting at the root composite (the one with the given name).
     */
    MemberTargetFinder(const std::string &compositeName, const std::list<int> memberIndices);

    AbstractTarget findTarget(Configuration *conf) const override;

};


#endif //AUA_MEMBERTARGETFINDER_H
