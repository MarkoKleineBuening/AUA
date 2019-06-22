//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/NonMemberTargetFinder.h"

NonMemberTargetFinder::NonMemberTargetFinder(const std::string &targetName) : targetName(targetName) {}

AbstractTarget NonMemberTargetFinder::findTarget(Configuration *conf) const {

    AbstractReference* targetBase;
    int size;

    if (conf->vars.find(targetName) != conf->vars.end()) {

        VarRef* targetVar = conf->vars[targetName];
        size = targetVar->getSize();
        targetBase = targetVar;

    } else {

        targetBase = conf->pointers[targetName];
        size = 0;

    }


    AbstractTarget target = AbstractTarget(targetBase, 0, size);

    return target;

}
