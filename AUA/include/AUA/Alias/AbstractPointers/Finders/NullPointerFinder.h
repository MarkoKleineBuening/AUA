//
// Created by mlaupichler on 15.09.19.
//

#ifndef AUA_NULLPOINTERFINDER_H
#define AUA_NULLPOINTERFINDER_H


#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>

class NullPointerFinder : public PointerFinder {

public:

    explicit NullPointerFinder(const PointerFormat &expectedFormat);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_NULLPOINTERFINDER_H
