//
// Created by mlaupichler on 15.09.19.
//

#ifndef AUA_NULLTARGETFINDER_H
#define AUA_NULLTARGETFINDER_H


#include <AUA/Alias/AbstractPointers/Finders/TargetFinder.h>

class NullTargetFinder : public TargetFinder{

public:

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_NULLTARGETFINDER_H
