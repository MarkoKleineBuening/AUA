//
// Created by mlaupichler on 15.09.19.
//

#ifndef AUA_COMPLETEARRAYCOMPOSITEFINDER_H
#define AUA_COMPLETEARRAYCOMPOSITEFINDER_H


#include <llvm/IR/Instructions.h>
#include "CompositeFinder.h"

class CompleteArrayCompositeFinder : public CompositeFinder {

private:

    CompositeFinder* parentArrayFinder;

    llvm::GetElementPtrInst* gepInst;

public:

    CompleteArrayCompositeFinder(CompositeFinder *parentArrayFinder,
                                 const CompositeFormat &expectedFormat,
                                 llvm::GetElementPtrInst *gepInst);

    CompositeSetValue *findComposites(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_COMPLETEARRAYCOMPOSITEFINDER_H
