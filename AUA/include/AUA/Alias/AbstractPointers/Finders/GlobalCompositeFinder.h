//
// Created by mlaupichler on 28.07.19.
//

#ifndef AUA_GLOBALCOMPOSITEFINDER_H
#define AUA_GLOBALCOMPOSITEFINDER_H


#include "CompositeFinder.h"
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>

class GlobalCompositeFinder : public CompositeFinder {

private:

    std::string name;

public:

    GlobalCompositeFinder(std::string name, const CompositeFormat &expectedFormat);

    CompositeSetValue *findComposites(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_GLOBALCOMPOSITEFINDER_H
