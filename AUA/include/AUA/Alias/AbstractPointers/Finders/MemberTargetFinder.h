//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_MEMBERTARGETFINDER_H
#define AUA_MEMBERTARGETFINDER_H


#include "TargetFinder.h"

class MemberTargetFinder : public TargetFinder {

private:

    const std::string compositeName;
    const int memberIdx;

public:

    MemberTargetFinder(const std::string &compositeName, const int memberIdx);

    AbstractTarget findTarget(Configuration *conf) const override;

};


#endif //AUA_MEMBERTARGETFINDER_H
