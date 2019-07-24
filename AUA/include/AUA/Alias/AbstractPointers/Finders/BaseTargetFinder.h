//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_BASETARGETFINDER_H
#define AUA_BASETARGETFINDER_H


#include "TargetFinder.h"

class BaseTargetFinder : public TargetFinder {

private:

    const std::string targetName;

public:

    explicit BaseTargetFinder(const std::string &targetName);

private:
    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

};


#endif //AUA_BASETARGETFINDER_H
