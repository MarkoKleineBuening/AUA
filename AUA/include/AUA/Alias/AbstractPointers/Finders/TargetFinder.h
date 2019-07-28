//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_TARGETFINDER_H
#define AUA_TARGETFINDER_H


#include <AUA/Alias/AbstractPointers/AbstractTarget.h>
#include <AUA/Alias/AbstractPointers/Configuration.h>

class TargetFinder {

public:

    virtual std::set<AbstractTarget> findTargets(Configuration *conf) const = 0;

    virtual std::list<const llvm::Instruction*> getAssociatedInsts() const  = 0;

};


#endif //AUA_TARGETFINDER_H
