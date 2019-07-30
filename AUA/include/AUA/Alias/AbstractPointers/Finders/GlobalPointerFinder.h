//
// Created by mlaupichler on 28.07.19.
//

#ifndef AUA_GLOBALPOINTERFINDER_H
#define AUA_GLOBALPOINTERFINDER_H


#include "PointerFinder.h"
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>


class GlobalPointerFinder : public PointerFinder {

private:

    std::string name;

public:

    GlobalPointerFinder(std::string name, const PointerFormat &expectedFormat);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_GLOBALPOINTERFINDER_H
