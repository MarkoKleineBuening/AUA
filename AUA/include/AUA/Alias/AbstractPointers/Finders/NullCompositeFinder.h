//
// Created by mlaupichler on 15.09.19.
//

#ifndef AUA_NULLCOMPOSITEFINDER_H
#define AUA_NULLCOMPOSITEFINDER_H


#include <AUA/Alias/AbstractPointers/Finders/CompositeFinder.h>

class NullCompositeFinder : public CompositeFinder {

public:
    explicit NullCompositeFinder(const CompositeFormat &expectedFormat);

    CompositeSetValue *findComposites(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};



#endif //AUA_NULLCOMPOSITEFINDER_H
