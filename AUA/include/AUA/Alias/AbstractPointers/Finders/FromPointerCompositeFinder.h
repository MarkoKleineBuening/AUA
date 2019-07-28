//
// Created by mlaupichler on 18.07.19.
//

#ifndef AUA_FROMPOINTERCOMPOSITEFINDER_H
#define AUA_FROMPOINTERCOMPOSITEFINDER_H


#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include <llvm/IR/Instructions.h>
#include "CompositeFinder.h"

class FromPointerCompositeFinder : public CompositeFinder {

private:

    const PointerFinder *pointerFinder;

    const llvm::LoadInst* loadInst;

public:

    FromPointerCompositeFinder(const PointerFinder *pointerFinder, const CompositeFormat& expectedFormat,
                               const llvm::LoadInst *loadInst);

    CompositeSetValue *findComposites(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;
};


#endif //AUA_FROMPOINTERCOMPOSITEFINDER_H
