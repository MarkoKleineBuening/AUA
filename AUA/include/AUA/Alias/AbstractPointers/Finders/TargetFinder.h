//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_TARGETFINDER_H
#define AUA_TARGETFINDER_H


#include <AUA/Alias/AbstractPointers/AbstractTarget.h>
#include <AUA/Alias/AbstractPointers/Configuration.h>

class TargetFinder {

public:

    virtual AbstractTarget findTarget(Configuration* conf) const = 0;

};


#endif //AUA_TARGETFINDER_H
