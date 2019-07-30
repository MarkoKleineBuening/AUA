//
// Created by mlaupichler on 28.07.19.
//

#ifndef AUA_GLOBALTARGETFINDER_H
#define AUA_GLOBALTARGETFINDER_H


#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>
#include "TargetFinder.h"


class GlobalTargetFinder : public TargetFinder {

private:

    std::string name;

public:

    explicit GlobalTargetFinder(const std::string &name);

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_GLOBALTARGETFINDER_H
