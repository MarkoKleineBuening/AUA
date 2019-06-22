//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_NONMEMBERTARGETFINDER_H
#define AUA_NONMEMBERTARGETFINDER_H


#include "TargetFinder.h"

class NonMemberTargetFinder : public TargetFinder {

private:

    const std::string targetName;

public:

    NonMemberTargetFinder(const std::string &targetName);

private:
    AbstractTarget findTarget(Configuration *conf) const override;

};


#endif //AUA_NONMEMBERTARGETFINDER_H
